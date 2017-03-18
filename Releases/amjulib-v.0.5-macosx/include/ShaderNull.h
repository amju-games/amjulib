#ifndef SHADER_NULL_H
#define SHADER_NULL_H

#include "Shader.h"

namespace Amju
{
class ShaderNull : public Shader
{
public:
  virtual bool Load(const std::string&) { return true; }

  virtual void Begin() override {}
  virtual void End() override {}

  virtual void Set(const std::string& name, const float matrix[16]) override {}
  virtual void Set(const std::string& name, float f) override {}
  virtual void Set(const std::string& name, const AmjuGL::Vec3&) override {}
  virtual void Set(const std::string& name, const Colour&) override {}
  virtual void Set(const std::string& name, AmjuGL::TextureHandle) override {}
  virtual void SetMatrix3x3(const std::string& name, const float matrix[9]) override {}
  virtual void SetInt(const std::string& name, int i) override {}
  virtual void UseThisShader() override {} 
  virtual int FindUniformLocation(const std::string& uniformName) override { return -1; }
  virtual int FindAttribLocation(const std::string& attribName) override { return -1; }
};
}

#endif
