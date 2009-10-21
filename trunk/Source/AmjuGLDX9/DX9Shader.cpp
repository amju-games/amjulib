#include "AmjuFirst.h"
#include "DX9Shader.h"
#include "AmjuGL-DX9.h"
#include "Assert.h"
#include "AmjuFinal.h"

namespace Amju
{
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

  Assert(AmjuGLDX9::dd);

/*
  // Load FX file using Amju File classes
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
  ID3DXBuffer* errors = 0;
  HRESULT r = D3DXCreateEffectFromFileA(AmjuGLDX9::dd, fxFileName.c_str(), 
    NULL, NULL, dwShaderFlags, NULL, &m_pEffect, &errors);

  if (errors)
  {
    m_errorStr = (char*)errors->GetBufferPointer();
    MessageBoxA(0, m_errorStr.c_str(), "Oh no", MB_ICONSTOP);
  }
  if (!SUCCEEDED(r))
  {
    Assert(0);
    // If !errors, file does not exist ?
    return false;
  }
  return true;
}

std::string DX9Shader::GetErrorString()
{
  return m_errorStr;
}

int DX9Shader::GetNumPasses() const
{
  AMJU_CALL_STACK;

  return m_numPasses;
}

void DX9Shader::Begin()
{
  AMJU_CALL_STACK;
  // Technique name is hardcoded. Only one pass.
  Begin("myTechnique");
  Assert(GetNumPasses() == 1);
  BeginPass(0);
}

void DX9Shader::Begin(const std::string& techniqueName)
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

  m_pEffect->SetTechnique(techniqueName.c_str()); // "RenderSceneWithTexture1Light" );
  m_pEffect->Begin(&m_numPasses, 0); // sets num passes

}

void DX9Shader::End()
{
  AMJU_CALL_STACK;

  if (!m_pEffect)
  {
    return;
  }
  // Curently only one pass, so end it here, no need for another function
  m_pEffect->EndPass();
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

void DX9Shader::Set(const std::string& name, const float matrix[16])
{
  D3DXHANDLE h = m_pEffect->GetParameterByName(0, name.c_str());
  if (!h)
  {
      return;
  }

  D3DXMATRIX m(matrix);
  if (FAILED(m_pEffect->SetMatrix(h, &m)))
  {
    Assert(0);
  }
}

void DX9Shader::Set(const std::string& name, float f)
{
  D3DXHANDLE h = m_pEffect->GetParameterByName(0, name.c_str());
  if (!h)
  {
      return;
  }

  if (FAILED(m_pEffect->SetFloat(h, f)))
  {
    Assert(0);
  }
}

void DX9Shader::Set(const std::string& name, const AmjuGL::Vec3& v)
{
  D3DXHANDLE h = m_pEffect->GetParameterByName(0, name.c_str());
  if (!h)
  {
      return;
  }

  if (FAILED(m_pEffect->SetValue(h, &v, sizeof(AmjuGL::Vec3))))
  {
    Assert(0);
  }
}

void DX9Shader::Set(const std::string& name, const Colour& c)
{
  D3DXHANDLE h = m_pEffect->GetParameterByName(0, name.c_str());
  if (!h)
  {
      return;
  }

  if (FAILED(m_pEffect->SetValue(h, &c, sizeof(Colour))))
  {
    Assert(0);
  }
}

void DX9Shader::Set(const std::string& name, AmjuGL::TextureHandle texId)
{
  D3DXHANDLE h = m_pEffect->GetParameterByName(0, name.c_str());
  if (!h)
  {
      return;
  }

  LPDIRECT3DTEXTURE9 t = reinterpret_cast<LPDIRECT3DTEXTURE9>(texId);

  if (FAILED(m_pEffect->SetTexture(h, t)))
  {
    Assert(0);
  }
}

}
