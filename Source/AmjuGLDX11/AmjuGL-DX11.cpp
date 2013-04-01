#include <AmjuFirst.h>
#ifdef AMJU_OFFER_DX11

#include "AmjuGL-DX11.h"
#include <D3DX11tex.h>
#include <AmjuGLWindowInfo.h>
#include <WinScreen.h>
#include "DX11Shader.h"
#include <ShaderNull.h>
#include <DegRad.h>
#include <stack>
#include <AmjuFinal.h>

namespace Amju
{
// Lots of this code is pasted from DXSDK tutorials

struct ConstantBuffer
{
  XMMATRIX mView;
  XMMATRIX mProjection;

  // TODO
  //XMFLOAT4 vLightDir[2];
  //XMFLOAT4 vLightColor[2];
};

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT AmjuGLDX11::InitDevice()
{
  HRESULT hr = S_OK;

  RECT rc;
  GetClientRect( g_hWnd, &rc );
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE( driverTypes );

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory( &sd, sizeof( sd ) );
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
      D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
    if( SUCCEEDED( hr ) )
      break;
  }
  if( FAILED( hr ) )
    return hr;

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = NULL;
  hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
  if( FAILED( hr ) )
    return hr;

  hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  pBackBuffer->Release();
  if( FAILED( hr ) )
    return hr;

  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory( &descDepth, sizeof(descDepth) );
  descDepth.Width = width;
  descDepth.Height = height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
  if( FAILED( hr ) )
    return hr;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory( &descDSV, sizeof(descDSV) );
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if( FAILED( hr ) )
    return hr;

  // TODO There seems to be something missing here - i.e. what is the depth
  //  test to use, etc. Check out 
  //  http://www.rastertek.com/dx11tut03.html

  g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  g_pImmediateContext->RSSetViewports( 1, &vp );

  // Create the constant buffer
  D3D11_BUFFER_DESC bd;
  ZeroMemory( &bd, sizeof(bd) );
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(ConstantBuffer);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
  if (FAILED(hr))
    return hr;

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  if (FAILED(hr))
    return hr;

  return S_OK;
}

void AmjuGLDX11::CleanupDevice()
{
  if( g_pImmediateContext ) g_pImmediateContext->ClearState();

  if( g_pSamplerLinear ) g_pSamplerLinear->Release();
  if( g_pTextureRV ) g_pTextureRV->Release();
  if( g_pConstantBuffer ) g_pConstantBuffer->Release();
  if( g_pRenderTargetView ) g_pRenderTargetView->Release();
  if( g_pDepthStencil ) g_pDepthStencil->Release();
  if( g_pDepthStencilView ) g_pDepthStencilView->Release();
  if( g_pSwapChain ) g_pSwapChain->Release();
  if( g_pImmediateContext ) g_pImmediateContext->Release();
  if( g_pd3dDevice ) g_pd3dDevice->Release();
}

AmjuGLDX11::AmjuGLDX11(WNDPROC wndproc)
{
  AMJU_CALL_STACK;

  g_hInst = NULL;
  g_hWnd = NULL;
  g_driverType = D3D_DRIVER_TYPE_NULL;
  g_featureLevel = D3D_FEATURE_LEVEL_11_0;
  g_pd3dDevice = NULL;
  g_pImmediateContext = NULL;
  g_pSwapChain = NULL;
  g_pRenderTargetView = NULL;
  g_pConstantBuffer = NULL;

  m_wndproc = wndproc;
  m_defaultShader = 0;
  m_currentShader = 0;

  g_pDepthStencil = NULL;
  g_pDepthStencilView = NULL;

  g_pTextureRV = NULL;
  g_pSamplerLinear = NULL;

  s_matrixMode = AmjuGL::AMJU_MATRIX_NOTSETYET;

}

AmjuGLDX11::~AmjuGLDX11()
{
  CleanupDevice();
  delete m_defaultShader;
}

bool AmjuGLDX11::CreateWindow(AmjuGLWindowInfo* w)
{
  static LPCWSTR AMJU_WINDOW_CLASS_NAME = L"MY_WINDOWS_CLASS";
  Assert(m_wndproc);

  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof( WNDCLASSEX );
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = m_wndproc; 
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = g_hInst; 
  wcex.hIcon = 0; 
  wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = AMJU_WINDOW_CLASS_NAME; 
  wcex.hIconSm = 0; 
  if( !RegisterClassEx( &wcex ) )
      return false; 

  HWND hWnd = CreateWindowEx(
    NULL, 
    AMJU_WINDOW_CLASS_NAME, 
    L"Amju",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,		
    0, 
    0, 
    w->GetWidth(), 
    w->GetHeight(), 
    NULL, 
    NULL, 
    g_hInst, 
    NULL);
 
  if (!hWnd)
  {
    return false; 
  }

  SetHWnd(hWnd);
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  g_hWnd = hWnd;

  return InitDevice() == S_OK;
}

void AmjuGLDX11::BeginScene()
{
  AMJU_CALL_STACK;
}

void AmjuGLDX11::EndScene()
{
  AMJU_CALL_STACK;
}

void AmjuGLDX11::Flip()
{
  g_pSwapChain->Present( 0, 0 );
}

void AmjuGLDX11::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;
  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)w;
  vp.Height = (FLOAT)h;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0; // TODO
  vp.TopLeftY = 0; // TODO
  g_pImmediateContext->RSSetViewports( 1, &vp );
}

void AmjuGLDX11::Init()
{
  AMJU_CALL_STACK;

  m_defaultShader = (DX11Shader*)LoadShader("dx11default.fx");
  Assert(m_defaultShader);
  m_currentShader = m_defaultShader;
}

void AmjuGLDX11::UseShader(DX11Shader* shader)
{
  if (shader)
  {
    m_currentShader = shader;
  }
  else
  {
    m_currentShader = m_defaultShader;  
  }
}

void AmjuGLDX11::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  // clear the backbuffer
  float ClearColor[4] = { clearR, clearG, clearB, 1.0f }; //red,green,blue,alpha
  g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
  // And z buffer
  g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void AmjuGLDX11::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  // Use RH version for compatibility with gluPerspective() etc
  g_Projection = XMMatrixPerspectiveFovLH(DegToRad(fov), aspectRatio, nearDist, farDist);
}

void AmjuGLDX11::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  XMVECTOR Eye = XMVectorSet(eyeX, eyeY, eyeZ, 0);
  XMVECTOR At = XMVectorSet(x, y, z, 0);
  XMVECTOR Up = XMVectorSet(upX, upY, upZ, 0);
  // Use RH Version
  g_View = XMMatrixLookAtLH(Eye, At, Up);
}

void AmjuGLDX11::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

}

struct AmjuVertex
{
  XMFLOAT3 pos;
  XMFLOAT3 normal;
  XMFLOAT2 uv;
};

void AmjuGLDX11::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;

}

void AmjuGLDX11::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;
  Assert(m_currentShader);

  int numTris = tris.size();

  // TODO Would be much better to create these once per mesh and reuse rather than
  //  continually recreating the buffers :-(
  // This is super inefficient but DX isn't the platform I really care about...
  ID3D11InputLayout*      g_pVertexLayout = NULL;
  ID3D11Buffer*           g_pVertexBuffer = NULL;

  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(AmjuVertex) * numTris * 3;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));
  InitData.pSysMem = &tris[0]; //vertices;
  HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  if (FAILED(hr))
    return;

  // Set vertex buffer
  UINT stride = sizeof(AmjuVertex);
  UINT offset = 0;
  g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

  // Set primitive topology
  g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
  hr = g_pd3dDevice->CreateInputLayout( layout, numElements, m_currentShader->GetBufferPointer(),
                                          m_currentShader->GetBufferSize(), &g_pVertexLayout );
  // TODO
  //pVSBlob->Release();

  if( FAILED( hr ) )
        return;

  // Set the input layout
  g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

  ConstantBuffer cb;
  ///cb.mWorld = XMMatrixTranspose( g_World );
  cb.mView = XMMatrixTranspose( g_View ); // TODO Why transpose
  cb.mProjection = XMMatrixTranspose( g_Projection );
  g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

  m_currentShader->SetShader(g_pImmediateContext);

  g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );

  // Texturing
  g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
  g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );

  g_pImmediateContext->Draw(numTris * 3, 0);

  if( g_pVertexBuffer ) g_pVertexBuffer->Release();
  if( g_pVertexLayout ) g_pVertexLayout->Release();
}

void AmjuGLDX11::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLDX11::SetMatrixMode(AmjuGL::MatrixMode m) 
{
  AMJU_CALL_STACK;

  s_matrixMode = m;
}

void AmjuGLDX11::SetIdentity()
{
  AMJU_CALL_STACK;

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_View = XMMatrixIdentity();
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    g_Projection = XMMatrixIdentity();
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX11::MultMatrix(const float m[16])
{
  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_View = XMMATRIX(m) * g_View;  // TODO Check this
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    Assert(0);
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX11::PushMatrix()
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode != AmjuGL::AMJU_MATRIX_NOTSETYET);
  
  // Yechh, better to just always deal with the top of the current stack
  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    mtxStack[(int)s_matrixMode].push(g_View);
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    mtxStack[(int)s_matrixMode].push(g_Projection);
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX11::PopMatrix()
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode != AmjuGL::AMJU_MATRIX_NOTSETYET);
  Assert(!mtxStack[(int)s_matrixMode].empty());

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_View = mtxStack[(int)s_matrixMode].top();
    mtxStack[(int)s_matrixMode].pop();
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    g_Projection = mtxStack[(int)s_matrixMode].top();
    mtxStack[(int)s_matrixMode].pop();
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX11::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);
  g_View = XMMatrixTranslation(x, y, z) * g_View;
}

void AmjuGLDX11::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);
  g_View = XMMatrixScaling(x, y, z) * g_View;
  // It isn't "XMMatrixTranslating" and "XMMatrixRotating", you MS asshats
}

void AmjuGLDX11::RotateX(float degs)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);
  g_View = XMMatrixRotationX(DegToRad(degs)) * g_View;
}

void AmjuGLDX11::RotateY(float degs)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);
  g_View = XMMatrixRotationY(DegToRad(degs)) * g_View;
}

void AmjuGLDX11::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_View = XMMatrixRotationZ(DegToRad(degs)) * g_View;
  }
  else
  {
    g_Projection = XMMatrixRotationZ(DegToRad(degs)) * g_Projection;
  }
}

void AmjuGLDX11::GetMatrix(AmjuGL::MatrixMode mm, float result[16])
{
  AMJU_CALL_STACK;

  switch (mm)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    memcpy(result, &g_View, 16 * sizeof(float)); 
    break;

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    memcpy(result, &g_Projection, 16 * sizeof(float));     
    break;
  
  case AmjuGL::AMJU_TEXTURE_MATRIX:
    // Not supported
    break;
  }
}

void AmjuGLDX11::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

}

void AmjuGLDX11::PopAttrib()
{
  AMJU_CALL_STACK;

}

void AmjuGLDX11::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_DEPTH_WRITE:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); //EnableZWrite(true);
    break;

  case AmjuGL::AMJU_DEPTH_READ:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    break;

  case AmjuGL::AMJU_LIGHTING:
    //dd->SetRenderState(D3DRS_LIGHTING, TRUE);
    //dd->LightEnable( 0, TRUE );
    //{
    //  const float A = 0.5f;
    //  dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(A, A, A, 1.0f ) );
    //}
    break;

  case AmjuGL::AMJU_BLEND:
    //dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    break;

  case AmjuGL::AMJU_TEXTURE_2D:
//    dd->SetTexture(0, mostRecentlyUsedTexture); // TODO
    break;
  }
}

void AmjuGLDX11::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_DEPTH_WRITE:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AmjuGL::AMJU_DEPTH_READ:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AmjuGL::AMJU_LIGHTING:
    // TODO TEMP TEST
    // If lighting is disabled, set the ambient colour to white.
    // Then the material colour's ambient value will be used.
    //dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
//    dd->SetRenderState(D3DRS_LIGHTING, FALSE);
//    dd->LightEnable( 0, FALSE);
    break;

  case AmjuGL::AMJU_BLEND:
    //dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    break;

  case AmjuGL::AMJU_TEXTURE_2D:
    //dd->SetTexture(0, 0); 
    break;
  }
}

void AmjuGLDX11::DestroyTextureHandle(AmjuGL::TextureHandle*)
{
  AMJU_CALL_STACK;

  // TODO Release texture 
}

static void CopyRGBATexture(uint8* texels, int pitch, const uint8* data, int width, int height)
{
  AMJU_CALL_STACK;

  for (int y = 0; y < height; y++)
  {
    uint8* row = texels + y * pitch;

    for (int x = 0; x < width; x++)
    {
      row[0] = data[2]; 
      row[1] = data[1];
      row[2] = data[0];
      row[3] = data[3]; // Alpha

      row += 4;
      data += 4;
    }
  }
}

static void CopyRGBTexture(uint8* texels, int pitch, const uint8* data, int width, int height)
{
  AMJU_CALL_STACK;

  for (int y = 0; y < height; y++)
  {
    uint8* row = texels + y * pitch;

    for (int x = 0; x < width; x++)
    {
        // RGB -> BGR
      row[0] = data[0];
      row[1] = data[1];
      row[2] = data[2];
      // RGB texture still has 16 bytes per pixel apprently.
      row[3] = 0;
      row += 4;
      data += 3;
    }
  }
}

void AmjuGLDX11::SetTexture(
  AmjuGL::TextureHandle* th, 
  AmjuGL::TextureType tt, 
  AmjuGL::TextureDepth td,  
  int width, 
  int height, 
  uint8* data)
{
  AMJU_CALL_STACK;

  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  ID3D11Texture2D *pTexture = NULL;

  // TODO If RGB, bulk data out to 32 bits
  uint8* padded = 0;
  if (td == AmjuGL::AMJU_RGB)
  {
    padded = new uint8[4 * width * height];
    CopyRGBTexture(padded, width * 4, data, width, height);
  }

  D3D11_SUBRESOURCE_DATA subr;
  subr.pSysMem = padded ? padded : data;
  subr.SysMemPitch = width * 4;
  subr.SysMemSlicePitch = 0;
  HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &subr, &pTexture);

  // Ok to del padded data here ?
  delete [] padded;

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  D3D11_RESOURCE_DIMENSION type;
  pTexture->GetType(&type);
  Assert(type == D3D11_RESOURCE_DIMENSION_TEXTURE2D);
		
  srvDesc.Format = desc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

  hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &g_pTextureRV);

  *th = reinterpret_cast<int>(g_pTextureRV); // echh...
}

void AmjuGLDX11::UseTexture(AmjuGL::TextureHandle th)
{
  AMJU_CALL_STACK;

  g_pTextureRV = reinterpret_cast<ID3D11ShaderResourceView*>(th);
}

void AmjuGLDX11::SetTextureType(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;
}

void AmjuGLDX11::SetTextureMode(AmjuGL::TextureMode tm)
{
  AMJU_CALL_STACK;
}

void AmjuGLDX11::SetTextureFilter(AmjuGL::TextureFilter tf)
{
  AMJU_CALL_STACK;
}

void AmjuGLDX11::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;

  //IDirect3DSurface9* pSurface = 0;

  //dd->GetBackBuffer(
  //  0, // UINT  iSwapChain,
  //  0, // UINT BackBuffer,
  //  D3DBACKBUFFER_TYPE_MONO, // D3DBACKBUFFER_TYPE Type,
  //  &pSurface); //IDirect3DSurface9 ** ppBackBuffer

  //D3DSURFACE_DESC desc;
  //pSurface->GetDesc(&desc);

  //D3DLOCKED_RECT lockedRect;
  //HRESULT hr = pSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
  //if (SUCCEEDED(hr))
  //{
  //  // Do we know if we are windowed or not ? We have to crop the window edges if so.

  //  // Copy surface into buffer. 
  //  // NB Buffer format is R8G8B8
  //  //h = MIN(h, desc.Height);
  //  //w = MIN(w, desc.Width);
  //  for (int i = 0; i < h; i++)
  //  {
  //    unsigned char* rowStart = (unsigned char*)lockedRect.pBits + i * lockedRect.Pitch;
  //    unsigned char* bufStart = buffer + i * w * 3;
  //    for (int j = 0; j < w; j++)
  //    {
  //      bufStart[0] = rowStart[0]; 
  //      bufStart[1] = rowStart[1]; 
  //      bufStart[2] = rowStart[2];
  //      bufStart += 3;
  //      rowStart += 4;
  //    }
  //  }
  //}

  //pSurface->UnlockRect(); // even if LockRect failed ?
  //pSurface->Release(); // ?
}

void AmjuGLDX11::DrawLighting(
  const AmjuGL::LightColour& globalAmbient, 
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;

  //D3DLIGHT9         g_pLight0;

  //g_pLight0.Type = D3DLIGHT_DIRECTIONAL;

  //// Light direction seems to be backwards compared to OpenGL/Wii 
  //D3DXVECTOR3 vecDir = D3DXVECTOR3(-lightPos.m_x, lightPos.m_y, -lightPos.m_z);

  //D3DXVec3Normalize( (D3DXVECTOR3*)&g_pLight0.Direction, &vecDir);

  //// We can set global ambient with SetRenderState, OR set it here
  //const float A = 0.0f;
  //const float D = 1.0f;

  //g_pLight0.Ambient.r = A;
  //g_pLight0.Ambient.g = A;
  //g_pLight0.Ambient.b = A;
  //g_pLight0.Ambient.a = 1.0f;

  //g_pLight0.Diffuse.r = D;
  //g_pLight0.Diffuse.g = D;
  //g_pLight0.Diffuse.b = D;
  //g_pLight0.Diffuse.a = 1.0f;

  //// This would do something if we set a specular component in the material
  //const float SPEC = 0;
  //g_pLight0.Specular.r = SPEC;
  //g_pLight0.Specular.g = SPEC;
  //g_pLight0.Specular.b = SPEC;
  //g_pLight0.Specular.a = SPEC;

  //dd->SetLight( 0, &g_pLight0 );
  //dd->LightEnable( 0, TRUE );

  // Tutorial says:
  // Be careful when setting up ambient lighting. You can very easily wash 
  // out the material's specular highlights.
  // TODO does this do anything ?
//  dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1.0f ) );
}

Shader* AmjuGLDX11::LoadShader(const std::string& shaderFileName)
{
  DX11Shader* s = new DX11Shader;
  // TODO Add "dx9" to shader file name
  if (!s->Load(shaderFileName, g_pd3dDevice)) //// + ".fx"))
  {
    // TODO Report Error ? Or gracefully fall back to null shader ?
    delete s;
    return 0; //new ShaderNull;
  }
  return s;
}

} // namespace

#endif // AMJU_OFFER_DX11
