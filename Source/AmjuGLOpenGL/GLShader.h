#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <map>
#include <string>
#include "Shader.h"

namespace Amju
{
class GLShader : public Shader
{
public:
  GLShader();
  ~GLShader();

  virtual bool Load(const std::string& filenamePrefix);
  
  bool Create(const std::string& vertexSource, const std::string& fragmentSource);

  std::string GetErrorString();

  // Start using the shader
  void Begin();
  // Finish using the shader
  void End();

  virtual void SetMatrix3x3(const std::string& name, const float matrix[9]);
  virtual void Set(const std::string& name, const float matrix[16]);
  virtual void Set(const std::string& name, float f);
  virtual void Set(const std::string& name, const AmjuGL::Vec3&);
  virtual void Set(const std::string& name, const Colour&);
  virtual void Set(const std::string& name, AmjuGL::TextureHandle);
  virtual void SetInt(const std::string& name, int i);

  int GetProgHandle() const;
  
  int GetUniformLocation(const std::string& uniformName);
  int GetAttribLocation(const std::string& attribName);

private:
  uint32 m_vertexShaderHandle;
  uint32 m_fragmentShaderHandle;
  uint32 m_programHandle;
  std::string m_errorStr;

  // Cache uniform locations
  typedef std::map<std::string, int> LocationMap;
  LocationMap m_map;
};

}

#endif
