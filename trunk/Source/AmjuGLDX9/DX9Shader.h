#ifdef AMJU_OFFER_DX9

#ifndef DX9_SHADER_H
#define DX9_SHADER_H

#include <string>
#include <D3DX9Effect.h>
#include "Shader.h"

namespace Amju
{
class DX9Shader : public Shader
{
public:
  DX9Shader();
  ~DX9Shader();

  bool Load(const std::string& fxFileName);

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

private:
  ID3DXEffect* m_pEffect;
  unsigned int m_numPasses;
  std::string m_errorStr;
};

}

#endif // include guard

#endif // AMJU_OFFER_DX9