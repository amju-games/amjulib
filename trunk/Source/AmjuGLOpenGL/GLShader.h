#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <string>
#include "Shader.h"

namespace Amju
{
class GLShader : public Shader
{
public:
  GLShader();
  ~GLShader();

  virtual bool Load(const std::string&);

  std::string GetErrorString();

  // Start using the shader
  void Begin();
  // Finish using the shader
  void End();

  virtual void Set(const std::string& name, const float matrix[16]);
  virtual void Set(const std::string& name, float f);
  virtual void Set(const std::string& name, const AmjuGL::Vec3&);
  virtual void Set(const std::string& name, const Colour&);
  virtual void Set(const std::string& name, AmjuGL::TextureHandle);

private:
  bool Create(const std::string& vertexSource, const std::string& fragmentSource);

private:
  uint32 m_vertexShaderHandle;
  uint32 m_fragmentShaderHandle;
  uint32 m_programHandle;
  std::string m_errorStr;
};

}

#endif
