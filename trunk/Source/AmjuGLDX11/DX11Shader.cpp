#include "AmjuFirst.h"
#include "DX11Shader.h"
#include "AmjuGL-DX11.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "Assert.h"
#include "AmjuFinal.h"

namespace Amju
{
// From DXSDK tutorial
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;
  hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
    dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
  if( FAILED(hr) )
  {
    if (pErrorBlob)
    {
      std::cout << (char*)pErrorBlob->GetBufferPointer() << "\n";

      pErrorBlob->Release();
    }
    return hr;
  }
  if( pErrorBlob ) pErrorBlob->Release();

  return S_OK;
}

DX11Shader::DX11Shader()
{
  AMJU_CALL_STACK;
}

DX11Shader::~DX11Shader()
{
  AMJU_CALL_STACK;

  if(m_pVertexShader) m_pVertexShader->Release();
  if(m_pPixelShader) m_pPixelShader->Release();
}

bool DX11Shader::Load(const std::string& fxFileName, ID3D11Device* dd)
{
  AMJU_CALL_STACK;

  std::wstring wFileName(fxFileName.begin(), fxFileName.end());

  // Compile the vertex shader
  HRESULT hr = CompileShaderFromFile((WCHAR*)wFileName.c_str() /*L"Tutorial02.fx"*/, "VS", "vs_4_0", &pVSBlob );
  if( FAILED( hr ) )
  {
      MessageBox( NULL,
                  L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
      return false;
  }

	// Create the vertex shader
	hr = dd->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return false;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
  hr = CompileShaderFromFile((WCHAR*)wFileName.c_str(), "PS", "ps_4_0", &pPSBlob );
  if( FAILED( hr ) )
  {
      MessageBox( NULL,
                  L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
      return false;
  }

	// Create the pixel shader
	hr = dd->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return false;

  return true;
}

const void* DX11Shader::GetBufferPointer()
{
  return pVSBlob->GetBufferPointer();
}

size_t DX11Shader::GetBufferSize()
{
  return pVSBlob->GetBufferSize();
}

void DX11Shader::SetShader(ID3D11DeviceContext* ddc)
{
	ddc->VSSetShader( m_pVertexShader, NULL, 0 );
	ddc->PSSetShader( m_pPixelShader, NULL, 0 );
}

std::string DX11Shader::GetErrorString()
{
  return m_errorStr;
}

int DX11Shader::GetNumPasses() const
{
  AMJU_CALL_STACK;

  return m_numPasses;
}

void DX11Shader::Begin()
{
  AMJU_CALL_STACK;

}

void DX11Shader::Begin(const std::string& techniqueName)
{
  AMJU_CALL_STACK;


}

void DX11Shader::End()
{
  AMJU_CALL_STACK;

}

void DX11Shader::BeginPass(int pass)
{
  AMJU_CALL_STACK;

}

void DX11Shader::EndPass()
{
  AMJU_CALL_STACK;

}

void DX11Shader::Set(const std::string& name, const float matrix[16])
{
}

void DX11Shader::Set(const std::string& name, float f)
{
}

void DX11Shader::Set(const std::string& name, const AmjuGL::Vec3& v)
{
}

void DX11Shader::Set(const std::string& name, const Colour& c)
{
}

void DX11Shader::Set(const std::string& name, AmjuGL::TextureHandle texId)
{
}

}
