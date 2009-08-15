// TODO
// Shadows: broken, but in game code ?
//  - try OGL - there is a translate somewhere..?
// Env mapping! -- not directly supported in DX9 apparently... not in OGLES either?
// Alpha in SetColour
// Lighting: can see difference between lit and unlit sides
// But light position moves with camera!??
// Mouse pos: don't use z buffer -- then not an AmjuGL function.
// Get screenshot
// Create mipmap data
// Draw line, sphere
// Vertex buffers
// Improve API to batch quads/tris; use vertex buffers 

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include <d3dx9math.h>
#include "AmjuGL-DX9.h"
#include "AmjuFinal.h"

extern "C"
{
int 
gluUnProject(double winx, double winy, double winz,
	     const double model[16], const double proj[16],
	     const int viewport[4],
	     double * objx, double * objy, double * objz);
}

namespace Amju
{
static LPDIRECT3DDEVICE9 dd = 0;

static LPD3DXMATRIXSTACK g_matrixStack = NULL;

// Projection matrix
static D3DXMATRIX matProj;

static D3DVIEWPORT9 s_viewPort;

AmjuGLDX9::AmjuGLDX9(LPDIRECT3DDEVICE9 d3dDevice)
{
  AMJU_CALL_STACK;

  dd = d3dDevice;
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

void AmjuGLDX9::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  s_viewPort.X      = x;
  s_viewPort.Y      = y;
  s_viewPort.Width  = w;
  s_viewPort.Height = h;
  s_viewPort.MinZ   = 0.0f;
  s_viewPort.MaxZ   = 1.0f;

  dd->SetViewport(&s_viewPort);
}

void AmjuGLDX9::Init()
{
  AMJU_CALL_STACK;

  // Set cullmode to match Open GL
	dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

  Enable(AmjuGL::DEPTH_TEST);
  Disable(AmjuGL::LIGHTING);

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

    dd->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(clearR, clearG, clearB, 1.0f ), 1.0f, 0 );

    dd->BeginScene();

    g_matrixStack->LoadIdentity();
}

void AmjuGLDX9::SetPerspectiveProjection(float fov, float aspectRatio)
{
  AMJU_CALL_STACK;

  // Use RH version for compatibility with gluPerspective() etc
    D3DXMatrixPerspectiveFovRH(
      &matProj, 
      D3DXToRadian(fov), 
      aspectRatio, 
      0.1f,  // near
      100.0f); // far

    dd->SetTransform(D3DTS_PROJECTION, &matProj);
}

void AmjuGLDX9::SetOrthoProjection()
{
  AMJU_CALL_STACK;

  // TODO
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

void AmjuGLDX9::DrawSphere(const AmjuGL::Vec3& v, float r)
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

void AmjuGLDX9::SetMatrixMode(AmjuGL::MatrixMode)
{
  AMJU_CALL_STACK;

  // TODO Remove this from AmjuGL ??????
}

void AmjuGLDX9::SetIdentity()
{
  AMJU_CALL_STACK;

  // TODO Do we need this in AmjuGL ??
    g_matrixStack->LoadIdentity();
}

void AmjuGLDX9::PushMatrix()
{
  AMJU_CALL_STACK;

    g_matrixStack->Push();
}

void AmjuGLDX9::PopMatrix()
{
  AMJU_CALL_STACK;

    g_matrixStack->Pop();
}

void AmjuGLDX9::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

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

  // TODO Use current matrix
}

void AmjuGLDX9::RotateX(float degs)
{
  AMJU_CALL_STACK;

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);

  D3DXMatrixRotationX(&m, D3DXToRadian(degs)); 

  g_matrixStack->MultMatrixLocal( &m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::RotateY(float degs)
{
  AMJU_CALL_STACK;

  D3DXMATRIX m;
  D3DXMatrixIdentity(&m);

  D3DXMatrixRotationY(&m, D3DXToRadian(degs)); 

  g_matrixStack->MultMatrixLocal( &m);
  dd->SetTransform(D3DTS_WORLD, g_matrixStack->GetTop());
}

void AmjuGLDX9::RotateZ(float degs)
{
  AMJU_CALL_STACK;

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
  case AmjuGL::MODELVIEW:
    {
    const D3DXMATRIX* pm = g_matrixStack->GetTop();
    memcpy(result, pm, 16 * sizeof(float)); 
    }
    break;

  case AmjuGL::PROJECTION:
    memcpy(result, &matProj, 16 * sizeof(float));     
    break;
  
  case AmjuGL::TEXTURE:
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

void AmjuGLDX9::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::DEPTH_TEST:
    dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    break;

  case AmjuGL::LIGHTING:
    dd->SetRenderState(D3DRS_LIGHTING, TRUE);
    dd->LightEnable( 0, TRUE );
    {
      const float A = 0.5f;
      dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(A, A, A, 1.0f ) );
    }
    break;

  case AmjuGL::BLEND:
    break;
  }
}

void AmjuGLDX9::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::DEPTH_TEST:
    dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AmjuGL::LIGHTING:
    // TODO TEMP TEST
    // If lighting is disabled, set the ambient colour to white.
    // Then the material colour's ambient value will be used.
    dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
//    dd->SetRenderState(D3DRS_LIGHTING, FALSE);
//    dd->LightEnable( 0, FALSE);
    break;

  case AmjuGL::BLEND:
    break;
  }
}

void AmjuGLDX9::BlendFunc()
{
  AMJU_CALL_STACK;

  // TODO Remove from AmjuGL
}

void AmjuGLDX9::EnableZWrite(bool writeToZ)
{
  AMJU_CALL_STACK;

  // TODO Better function name
  dd->SetRenderState(D3DRS_ZENABLE, writeToZ ? D3DZB_TRUE : D3DZB_FALSE);
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
    (td == AmjuGL::RGB ? D3DFMT_R8G8B8 : D3DFMT_A8R8G8B8), //D3DFORMAT Format,
    D3DPOOL_DEFAULT, // Pool,
    reinterpret_cast<LPDIRECT3DTEXTURE9*>(th) //LPDIRECT3DTEXTURE9 * ppTexture
  );

  LPDIRECT3DTEXTURE9 pTex = reinterpret_cast<LPDIRECT3DTEXTURE9>(*th);

  D3DLOCKED_RECT lockedRect;
  pTex->LockRect(0, &lockedRect, NULL, 0);

  switch (td)
  {
  case AmjuGL::RGB:
    CopyRGBTexture((uint8*)lockedRect.pBits, lockedRect.Pitch, data, width, height); 
    break;
  case AmjuGL::RGBA:
    CopyRGBATexture((uint8*)lockedRect.pBits, lockedRect.Pitch, data, width, height); 
    break;
  }

  pTex->UnlockRect(0);

  // TODO Create data for each mipmap level.
  // NB We want the same functionality as screenshot shrinking.
  IDirect3DSurface9 * pSurfaceLevel;
  for (int iLevel = 0; iLevel < pTex->GetLevelCount(); iLevel++)
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

// Is this necessary ? -- I think so, can't just cast away this difference.
static void ConvertFloatToDoubleArray(const float src[16], double dest[16])
{
  AMJU_CALL_STACK;

  for (int i = 0; i < 16; i++)
  {
    dest[i] = (double)src[i];
  }
}

AmjuGL::Vec3 AmjuGLDX9::MouseToWorld(int x, int y)
{
/*

  GLint viewport[4];
  GLdouble modelview[16],projection[16];
  GLfloat wx=x,wy;
  GLfloat wz = 0;

  // NB Order: get the viewport, then invert y, then get depth value :-)
  glGetIntegerv(GL_VIEWPORT,viewport);
  y=viewport[3]-y;

  glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);

  wy = y;

  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
  glGetDoublev(GL_PROJECTION_MATRIX,projection);
  double ox, oy, oz;
  gluUnProject(wx, wy, wz,
    modelview,projection,viewport,&ox,&oy,&oz);

  return AmjuGL::Vec3(ox, oy, oz);

*/

  // We stored the viewport when it was set
  // NB This should be the window size, explains why mouse is wrong in letterbox..?
  y = s_viewPort.Height - y;
  /*
  // Get Z buffer at mouse coord
  IDirect3DSurface9* pSurface;
  if (GetDepthStencilSurface(&pSurface) != D3D_OK)
  {
    // Failed
    return AmjuGL::Vec3(0, 0, 0);
  }
  D3DLOCKED_RECT lockedRect;
  RECT r;
  r.left = x;
  r.right = x;
  r.top = y;
  r.bottom = y;
  pSurface->LockRect(&lockedRect, r, D3DLOCK_READONLY);


  pSurface->Release();
  */
  float wx = x;
  float wy = y;
  float wz = 0.5; // TODO depth
  float modelview[16];
  float proj[16];
  GetMatrix(AmjuGL::MODELVIEW, modelview);
  GetMatrix(AmjuGL::PROJECTION, proj);

  double ox, oy, oz;

  // Convert flat matrixes to double for gluUnproject -- I think we
  // must convert (or change gluUnproject) - we can't just cast.
  double modelviewD[16];
  double projD[16];
  ConvertFloatToDoubleArray(modelview, modelviewD);
  ConvertFloatToDoubleArray(proj, projD);

  gluUnProject(
    wx, wy, wz,
    modelviewD,
    projD,
    (int*)&s_viewPort.X,
    &ox,&oy,&oz);

  return AmjuGL::Vec3(ox, oy, oz);
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
  D3DXVECTOR3 vecDir = D3DXVECTOR3( 1.0f, -1.0f, -1.0f );
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

DX9Shader::DX9Shader()
{
  AMJU_CALL_STACK;

  m_pEffect = 0;
  m_numPasses = 0;
}

DX9Shader::~DX9Shader()
{
  AMJU_CALL_STACK;

  // TODO clean up
}

bool DX9Shader::Load(const std::string& fxFileName)
{
  AMJU_CALL_STACK;

/*
  // Load FX file
  // TODO
  File fxFile(false); // no version info
  if (!fxFile.OpenRead("BasicHLSL.fx.txt", true // binary 
  ))
  {
    fxFile.ReportError("Failed to load DX9 Effect file");
    return;
  }
  unsigned long filelen = fxFile.GetFileSize();

  unsigned char* fxSource = new unsigned char[filelen + 1];
  fxFile.GetBinary(filelen, fxSource);
*/

  // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
  // shader debugger. Debugging vertex shaders requires either REF or software vertex 
  // processing, and debugging pixel shaders requires REF.  The 
  // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
  // shader debugger.  It enables source level debugging, prevents instruction 
  // reordering, prevents dead code elimination, and forces the compiler to compile 
  // against the next higher available software target, which ensures that the 
  // unoptimized shaders do not exceed the shader model limitations.  Setting these 
  // flags will cause slower rendering since the shaders will be unoptimized and 
  // forced into software.  See the DirectX documentation for more information about 
  // using the shader debugger.
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

  #if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DXSHADER_DEBUG;
  #endif

  #ifdef DEBUG_VS
      dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
  #endif
  #ifdef DEBUG_PS
      dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
  #endif

  // Preshaders are parts of the shader that the effect system pulls out of the 
  // shader and runs on the host CPU. They should be used if you are GPU limited. 
  // The D3DXSHADER_NO_PRESHADER flag disables preshaders.
  if(false) // !g_bEnablePreshader )
      dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

  // If this fails, there should be debug output as to 
  // why the .fx file failed to compile
  HRESULT r = D3DXCreateEffectFromFile(dd, fxFileName.c_str(), 
    NULL, NULL, dwShaderFlags, NULL, &m_pEffect, NULL );
  if (!SUCCEEDED(r))
  {
    return false;
  }
  return true;
}

std::string DX9Shader::GetErrorString()
{
  return "TODO";
}

int DX9Shader::GetNumPasses() const
{
  AMJU_CALL_STACK;

  return m_numPasses;
}

void DX9Shader::Begin()
{
  AMJU_CALL_STACK;

  if (!m_pEffect)
  {
    return;
  }

  // Update the effect's variables.  Instead of using strings, it would 
  // be more efficient to cache a handle to the parameter by calling 
  // ID3DXEffect::GetParameterByName
  /*
  V( m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
  V( m_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
  V( m_pEffect->SetFloat( "g_fTime", (float)fTime ) );

  D3DXCOLOR vWhite = D3DXCOLOR(1,1,1,1);
  V( m_pEffect->SetValue("g_MaterialDiffuseColor", &vWhite, sizeof(D3DXCOLOR) ) );
  V( m_pEffect->SetFloat( "g_fTime", (float)fTime ) );      
  V( m_pEffect->SetInt( "g_nNumLights", g_nNumActiveLights ) );      

  V_RETURN( m_pEffect->SetValue("g_MaterialAmbientColor", &colorMtrlAmbient, sizeof(D3DXCOLOR) ) );
  V_RETURN( m_pEffect->SetValue("g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof(D3DXCOLOR) ) );    
  V_RETURN( m_pEffect->SetTexture( "g_MeshTexture", g_pMeshTexture) );
  */
  // Render the scene with this technique 
  // as defined in the .fx file
  /*
  switch( g_nNumActiveLights )
  {
      case 1: V( m_pEffect->SetTechnique( "RenderSceneWithTexture1Light" ) ); break;
      case 2: V( m_pEffect->SetTechnique( "RenderSceneWithTexture2Light" ) ); break;
      case 3: V( m_pEffect->SetTechnique( "RenderSceneWithTexture3Light" ) ); break;
  }
  */

  m_pEffect->SetTechnique( "RenderSceneWithTexture1Light" );
  m_pEffect->Begin(&m_numPasses, 0);

}

void DX9Shader::End()
{
  AMJU_CALL_STACK;

  if (!m_pEffect)
  {
    return;
  }
  m_pEffect->End();
}

void DX9Shader::BeginPass(int pass)
{
  AMJU_CALL_STACK;

  if (!m_pEffect)
  {
    return;
  }
  m_pEffect->BeginPass(pass);
}

void DX9Shader::EndPass()
{
  AMJU_CALL_STACK;

  if (!m_pEffect)
  {
    return;
  }
  m_pEffect->EndPass();
}

} // namespace
