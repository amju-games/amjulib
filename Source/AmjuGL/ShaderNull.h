#ifndef SHADER_NULL_H
#define SHADER_NULL_H

#include "Shader.h"

namespace Amju
{
class ShaderNull : public Shader
{
public:
  bool Load(const std::string&) override { return true; }

  void Begin() override {}
  void End() override {}

  void Set(const std::string& name, const float matrix[16]) override {}
  void Set(const std::string& name, float f) override {}
  void Set(const std::string& name, const AmjuGL::Vec3&) override {}
  void Set(const std::string& name, const Colour&) override {}
  void Set(const std::string& name, AmjuGL::TextureHandle) override {}
  void SetMatrix3x3(const std::string& name, const float matrix[9]) override {}
  void SetInt(const std::string& name, int i) override {}
  void UseThisShader() override {} 
  int FindUniformLocation(const std::string& uniformName) override { return -1; }
  int FindAttribLocation(const std::string& attribName) override { return -1; }
};
}

#endif
