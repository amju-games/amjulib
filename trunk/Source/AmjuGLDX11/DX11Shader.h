#ifndef DX11_SHADER_H
#define DX11_SHADER_H

#include <string>
#include <d3d11.h>
#include "Shader.h"

namespace Amju
{
class DX11Shader : public Shader
{
public:
  DX11Shader();
  ~DX11Shader();

  bool Load(const std::string& fxFileName, ID3D11Device* dd);

  std::string GetErrorString();

  // Start using the shader. Use Begin before any pass
  virtual void Begin(); // no technique name - for now, always hardcoded
  
  // Finish using the shader: call after all passes are finished
  void End();

  // For now, only one pass is supported
  // After calling begin, you can get the number of passes for this effect
  int GetNumPasses() const;
  // Use BeginPass/EndPass around each render pass
  void BeginPass(int passNum);
  void EndPass();

  void Set(const std::string& effectParamName, const float matrix[16]);
  void Set(const std::string& effectParamName, float f);
  void Set(const std::string& effectParamName, const AmjuGL::Vec3&);
  void Set(const std::string& effectParamName, const Colour&);
  void Set(const std::string& effectParamName, AmjuGL::TextureHandle);

private:
  void Begin(const std::string& techniqueName);

  // Used by AmjuGLDX11
  friend class AmjuGLDX11;
  const void* GetBufferPointer();
  size_t GetBufferSize();
  void SetShader(ID3D11DeviceContext*);

private:
  unsigned int m_numPasses;
  std::string m_errorStr;

  ID3D11VertexShader* m_pVertexShader;
  ID3D11PixelShader* m_pPixelShader;
  ID3DBlob* pVSBlob; // what is this for ?
};

}

#endif
