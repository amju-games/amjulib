// TODO
// Shadows: broken, but in game code ?
//  - try OGL - there is a translate somewhere..?
// Env mapping! -- not directly supported in DX9 apparently... not in OGLES either?
// Alpha in SetColour
// Lighting: can see difference between lit and unlit sides
// But light position moves with camera!??
// Get screenshot
// Create mipmap data
// Draw line
// Vertex buffers
// Improve API to batch quads/tris; use vertex buffers 

#include "AmjuFirst.h"
#include <d3dx9math.h>
#include "AmjuGL-DX9.h"
#include <WinScreen.h>
#include "DX9Shader.h"
#include <ShaderNull.h>
#include "AmjuFinal.h"

namespace Amju
{
LPDIRECT3DDEVICE9 AmjuGLDX9::dd = 0;

// Modelview matrix stack
static LPD3DXMATRIXSTACK g_matrixStack = NULL;

// Projection matrix
static D3DXMATRIX matProj;

static D3DVIEWPORT9 s_viewPort;

static AmjuGL::MatrixMode s_matrixMode = AmjuGL::AMJU_MODELVIEW_MATRIX;

AmjuGLDX9::AmjuGLDX9(WNDPROC wndproc)
{
  AMJU_CALL_STACK;

  m_wndproc = wndproc;
}

/*
void AmjuGLDX9::Shutdown()
{
  AMJU_CALL_STACK;

  // TODO
  // Add to AmjuGL
    if( g_matrixStack != NULL )
        g_matrixStack->Release();
}
*/

void AmjuGLDX9::BeginScene()
{
  AMJU_CALL_STACK;

  // Done in Init[Scene] so we clear buffers before calling [dx9]BeginScene
}

void AmjuGLDX9::EndScene()
{
  AMJU_CALL_STACK;

  dd->EndScene();
  dd->Present( NULL, NULL, NULL, NULL );
}

void AmjuGLDX9::Flip()
{
  //ValidateRect((HWND)GetHWnd(), 0);
}

void AmjuGLDX9::Viewport(int x, int y, int w, int h)
{
  /*
  AMJU_CALL_STACK;

  s_viewPort.X      = x;
  s_viewPort.Y      = y;
  s_viewPort.Width  = w;
  s_viewPort.Height = h;
  s_viewPort.MinZ   = 0.0f;
  s_viewPort.MaxZ   = 1.0f;

  dd->SetViewport(&s_viewPort);
  */
}

void AmjuGLDX9::Init()
{
  AMJU_CALL_STACK;

  // Set cullmode 
	dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

  Enable(AmjuGL::AMJU_DEPTH_READ);
  Disable(AmjuGL::AMJU_LIGHTING);

  D3DXCreateMatrixStack( 0, &g_matrixStack );

  // Set view matrix to identity and leave it alone
  D3DXMATRIX matView;
  D3DXMatrixIdentity(&matView);
  dd->SetTransform(D3DTS_VIEW, &matView);

  // Set texture sampling: if you don't do this you get horible aliased textures.
  // TODO Make sure mipmapping is enabled.
	dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  // Mipmap filter
  // This shows that mip mapping is on - LINEAR looks best.
  // TODO Turn this on when we create mip map data!
//  dd->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT); // LINEAR);

  // Set up blend mode
  dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  // Ignore vertex colours when doing lighting calculations
  //  -- makes no difference ??!
  dd->SetRenderState( D3DRS_COLORVERTEX, FALSE );
}

void AmjuGLDX9::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  dd->BeginScene();


  dd->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_COLORVALUE(clearR, clearG, clearB, 1.0f ), 1.0f, 0 );

  g_matrixStack->LoadIdentity();
}

void AmjuGLDX9::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  // Use RH version for compatibility with gluPerspective() etc
  D3DXMatrixPerspectiveFovRH(
    &matProj, 
    D3DXToRadian(fov), 
    aspectRatio, 
    nearDist,  
    farDist); 

  dd->SetTransform(D3DTS_PROJECTION, &matProj);
}

void AmjuGLDX9::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  D3DXVECTOR3 vEyePt(eyeX, eyeY, eyeZ);
  D3DXVECTOR3 vLookatPt(x, y, z);
  D3DXVECTOR3 vUpVec(upX, upY, upZ);
  D3DXMATRIX matLookAt;
  D3DXMatrixLookAtRH( &matLookAt, &vEyePt, &vLookatPt, &vUpVec );

  g_matrixStack->LoadIdentity();
  g_matrixStack->LoadMatrix( &matLookAt );
}

void AmjuGLDX9::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  // Set up a material -- do we need to do this???
  D3DMATERIAL9 material;

  ZeroMemory( &material, sizeof(D3DMATERIAL9) );

  material.Diffuse.r = r;
  material.Diffuse.g = g;
  material.Diffuse.b = b;
  material.Diffuse.a = a;

  material.Ambient.r = r;
  material.Ambient.g = g;
  material.Ambient.b = b;
  material.Ambient.a = a;

  material.Emissive.r = 0.0f;
  material.Emissive.g = 0.0f;
  material.Emissive.b = 0.0f;
  material.Emissive.a = 0.0f; // ???

  const float SPEC = 0;
  material.Specular.r = SPEC;
  material.Specular.g = SPEC;
  material.Specular.b = SPEC;
  material.Specular.a = SPEC;

  // Set the Power value to a small number to make the highlight's size bigger.
  material.Power = 5.0f;

  // Another way to ignore per-vertex colors is to simply force all lighting 
  // calculations to use your material settings explicitly like so...

  dd->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
//  dd->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL );
  dd->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
//  dd->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );

  dd->SetMaterial(&material);

}

static const uint32 D3DFVF_AMJU_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
struct AMJU_FVF
{
  D3DVECTOR pos;
  D3DVECTOR normal;
  float m_vecTex[2];
};

void AmjuGLDX9::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;

}

void AmjuGLDX9::DrawQuad(AmjuGL::Vert* verts)
{
  AMJU_CALL_STACK;

  AmjuGL::Tris tris;
  AmjuGL::Tri t1;
  t1.m_verts[0] = verts[0];
  t1.m_verts[1] = verts[1];
  t1.m_verts[2] = verts[2];
  tris.push_back(t1);
  AmjuGL::Tri t2;
  t2.m_verts[0] = verts[2];
  t2.m_verts[1] = verts[3];
  t2.m_verts[2] = verts[0];
  tris.push_back(t2);
  DrawTriList(tris);
}

void AmjuGLDX9::DrawQuadList(const AmjuGL::Quads& quads)
{
  AMJU_CALL_STACK;

  // TODO Quad List doesn't exist in DX9. Nor in GL ES, so we need a reusable
  //  function to convert to Tri list. 
}

void AmjuGLDX9::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;

  int numTris = tris.size();

  dd->SetFVF(D3DFVF_AMJU_VERTEX);
  dd->DrawPrimitiveUP(
    D3DPT_TRIANGLELIST, 
    numTris,
    &tris[0],
    sizeof(AMJU_FVF));
}

void AmjuGLDX9::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLDX9::SetMatrixMode(AmjuGL::MatrixMode m) 
{
  AMJU_CALL_STACK;

  s_matrixMode = m;
}

void AmjuGLDX9::SetIdentity()
{
  AMJU_CALL_STACK;

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_matrixStack->LoadIdentity();
    dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    D3DXMATRIX mat;
    D3DXMatrixIdentity(&mat);
    dd->SetTransform(D3DTS_PROJECTION, &mat);
  }
}

void AmjuGLDX9::MultMatrix(const float m[16])
{
  D3DXMATRIX d3dm(
    m[0], m[1], m[2],  m[3],
    m[4], m[5], m[6],  m[7],
    m[8], m[9], m[10], m[11],
    m[12], m[13], m[14], m[15]);

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_matrixStack->MultMatrixLocal(&d3dm);
    dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
  }
  else if (s_matrixMode == AmjuGL::AMJU_PROJECTION_MATRIX)
  {
    Assert(0);
  }
}

void AmjuGLDX9::PushMatrix()
{
  AMJU_CALL_STACK;

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_matrixStack->Push();
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX9::PopMatrix()
{
  AMJU_CALL_STACK;

  if (s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX)
  {
    g_matrixStack->Pop();
    D3DXMATRIX* m = g_matrixStack->GetTop();
    // We always load Identity before Pushing, right ?
    Assert(m);
    dd->SetTransform(D3DTS_WORLD,  m);
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLDX9::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);

  // TODO Use current matrix mode, or scrap the idea of 
  // manipulating proj and tex mats
  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);
  D3DXMatrixTranslation(&m, x, y, z);

  g_matrixStack->MultMatrixLocal( &m);
  // WORLD -- VIEW matrix is always just identity
  dd->SetTransform( D3DTS_WORLD, g_matrixStack->GetTop() );
}

void AmjuGLDX9::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);
  D3DXMatrixScaling(&m, x, y, z); 
  g_matrixStack->MultMatrixLocal(&m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::RotateX(float degs)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);
  D3DXMatrixRotationX(&m, D3DXToRadian(degs)); 
  g_matrixStack->MultMatrixLocal( &m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::RotateY(float degs)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);
  D3DXMatrixRotationY(&m, D3DXToRadian(degs)); 
  g_matrixStack->MultMatrixLocal( &m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  Assert(s_matrixMode == AmjuGL::AMJU_MODELVIEW_MATRIX);

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);
  D3DXMatrixRotationZ(&m, D3DXToRadian(degs)); 
  g_matrixStack->MultMatrixLocal( &m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::GetMatrix(AmjuGL::MatrixMode mm, float result[16])
{
  AMJU_CALL_STACK;

  switch (mm)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    {
    const D3DXMATRIX* pm = g_matrixStack->GetTop();
    memcpy(result, pm, 16 * sizeof(float)); 
    }
    break;

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    memcpy(result, &matProj, 16 * sizeof(float));     
    break;
  
  case AmjuGL::AMJU_TEXTURE_MATRIX:
    // Not supported
    break;
  }
}

void AmjuGLDX9::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

}

void AmjuGLDX9::PopAttrib()
{
  AMJU_CALL_STACK;

}

static void EnableZWrite(bool writeToZ)
{
  AMJU_CALL_STACK;

  // TODO Better function name
  AmjuGLDX9::dd->SetRenderState(D3DRS_ZENABLE, writeToZ ? D3DZB_TRUE : D3DZB_FALSE);
}

void AmjuGLDX9::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_DEPTH_WRITE:
    EnableZWrite(true);
    break;

  case AmjuGL::AMJU_DEPTH_READ:
    dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    break;

  case AmjuGL::AMJU_LIGHTING:
    dd->SetRenderState(D3DRS_LIGHTING, TRUE);
    dd->LightEnable( 0, TRUE );
    {
      const float A = 0.5f;
      dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(A, A, A, 1.0f ) );
    }
    break;

  case AmjuGL::AMJU_BLEND:
    break;
  }
}

void AmjuGLDX9::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_DEPTH_WRITE:
    EnableZWrite(false);
    break;

  case AmjuGL::AMJU_DEPTH_READ:
    dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AmjuGL::AMJU_LIGHTING:
    // TODO TEMP TEST
    // If lighting is disabled, set the ambient colour to white.
    // Then the material colour's ambient value will be used.
    dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
//    dd->SetRenderState(D3DRS_LIGHTING, FALSE);
//    dd->LightEnable( 0, FALSE);
    break;

  case AmjuGL::AMJU_BLEND:
    break;
  }
}

void AmjuGLDX9::DestroyTextureHandle(AmjuGL::TextureHandle*)
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
      row[0] = data[2];
      row[1] = data[1];
      row[2] = data[0];
      // RGB texture still has 16 bytes per pixel apprently.
      row[3] = 0;
      row += 4;
      data += 3;
    }
  }
}

void AmjuGLDX9::SetTexture(
  AmjuGL::TextureHandle* th, 
  AmjuGL::TextureType tt, 
  AmjuGL::TextureDepth td,  
  int width, 
  int height, 
  uint8* data)
{
  AMJU_CALL_STACK;

  HRESULT res = D3DXCreateTexture(
    dd, //LPDIRECT3DDEVICE9 pDevice,
    width, //UINT Width,
    height, //UINT Height,
    0, //  Mip Levels: 0 means create all
    D3DUSAGE_DYNAMIC, //DWORD Usage,
    (td == AmjuGL::AMJU_RGB ? D3DFMT_R8G8B8 : D3DFMT_A8R8G8B8), //D3DFORMAT Format,
    D3DPOOL_DEFAULT, // Pool,
    reinterpret_cast<LPDIRECT3DTEXTURE9*>(th) //LPDIRECT3DTEXTURE9 * ppTexture
  );

  LPDIRECT3DTEXTURE9 pTex = reinterpret_cast<LPDIRECT3DTEXTURE9>(*th);

  D3DLOCKED_RECT lockedRect;
  pTex->LockRect(0, &lockedRect, NULL, 0);

  switch (td)
  {
  case AmjuGL::AMJU_RGB:
    CopyRGBTexture((uint8*)lockedRect.pBits, lockedRect.Pitch, data, width, height); 
    break;
  case AmjuGL::AMJU_RGBA:
    CopyRGBATexture((uint8*)lockedRect.pBits, lockedRect.Pitch, data, width, height); 
    break;
  }

  pTex->UnlockRect(0);

  // TODO Create data for each mipmap level.
  // NB We want the same functionality as screenshot shrinking.
  IDirect3DSurface9 * pSurfaceLevel;
  for (unsigned int iLevel = 0; iLevel < pTex->GetLevelCount(); iLevel++)
  {
    pTex->GetSurfaceLevel(iLevel, &pSurfaceLevel);

    // TODO Write this mip map
    
    pSurfaceLevel->Release();
  }
}

void AmjuGLDX9::UseTexture(AmjuGL::TextureHandle th)
{
  AMJU_CALL_STACK;

  LPDIRECT3DTEXTURE9 pTex = reinterpret_cast<LPDIRECT3DTEXTURE9>(th);
  dd->SetTexture(0, pTex);

  // TODO Maybe we need to set that we want to modulate the current colour
  // with the texture.

  // Setup our texture. Using Textures introduces the texture stage states,
  // which govern how Textures get blended together (in the case of multiple
  // Textures) and lighting information. In this case, we are modulating
  // (blending) our texture with the diffuse color of the vertices.
//  dd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
//  dd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//  dd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
}

void AmjuGLDX9::SetTextureMode(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;

}

void AmjuGLDX9::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;

  IDirect3DSurface9* pSurface = 0;

  dd->GetBackBuffer(
    0, // UINT  iSwapChain,
    0, // UINT BackBuffer,
    D3DBACKBUFFER_TYPE_MONO, // D3DBACKBUFFER_TYPE Type,
    &pSurface); //IDirect3DSurface9 ** ppBackBuffer

  D3DSURFACE_DESC desc;
  pSurface->GetDesc(&desc);

  D3DLOCKED_RECT lockedRect;
  HRESULT hr = pSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
  if (SUCCEEDED(hr))
  {
    // Do we know if we are windowed or not ? We have to crop the window edges if so.

    // Copy surface into buffer. 
    // NB Buffer format is R8G8B8
    //h = MIN(h, desc.Height);
    //w = MIN(w, desc.Width);
    for (int i = 0; i < h; i++)
    {
      unsigned char* rowStart = (unsigned char*)lockedRect.pBits + i * lockedRect.Pitch;
      unsigned char* bufStart = buffer + i * w * 3;
      for (int j = 0; j < w; j++)
      {
        bufStart[0] = rowStart[0]; 
        bufStart[1] = rowStart[1]; 
        bufStart[2] = rowStart[2];
        bufStart += 3;
        rowStart += 4;
      }
    }
  }

  pSurface->UnlockRect(); // even if LockRect failed ?
  pSurface->Release(); // ?
}

void AmjuGLDX9::DrawLighting(
  const AmjuGL::LightColour& globalAmbient, 
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;

  D3DLIGHT9         g_pLight0;

  g_pLight0.Type = D3DLIGHT_DIRECTIONAL;

  // Light direction seems to be backwards compared to OpenGL/Wii 
  D3DXVECTOR3 vecDir = D3DXVECTOR3(-lightPos.m_x, lightPos.m_y, -lightPos.m_z);

  D3DXVec3Normalize( (D3DXVECTOR3*)&g_pLight0.Direction, &vecDir);

  // We can set global ambient with SetRenderState, OR set it here
  const float A = 0.0f;
  const float D = 1.0f;

  g_pLight0.Ambient.r = A;
  g_pLight0.Ambient.g = A;
  g_pLight0.Ambient.b = A;
  g_pLight0.Ambient.a = 1.0f;

  g_pLight0.Diffuse.r = D;
  g_pLight0.Diffuse.g = D;
  g_pLight0.Diffuse.b = D;
  g_pLight0.Diffuse.a = 1.0f;

  // This would do something if we set a specular component in the material
  const float SPEC = 0;
  g_pLight0.Specular.r = SPEC;
  g_pLight0.Specular.g = SPEC;
  g_pLight0.Specular.b = SPEC;
  g_pLight0.Specular.a = SPEC;

  dd->SetLight( 0, &g_pLight0 );
  dd->LightEnable( 0, TRUE );

  // Tutorial says:
  // Be careful when setting up ambient lighting. You can very easily wash 
  // out the material's specular highlights.
  // TODO does this do anything ?
//  dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1.0f ) );
}

Shader* AmjuGLDX9::LoadShader(const std::string& shaderFileName)
{
  DX9Shader* s = new DX9Shader;
  // TODO Add "dx9" to shader file name
  if (!s->Load(shaderFileName + ".fx"))
  {
    // TODO Report Error ? Or gracefully fall back to null shader ?
    delete s;
    return new ShaderNull;
  }
  return s;
}

} // namespace
