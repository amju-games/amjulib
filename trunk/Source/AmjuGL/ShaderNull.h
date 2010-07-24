#ifndef SHADER_NULL_H
#define SHADER_NULL_H

#include "Shader.h"

namespace Amju
{
class ShaderNull : public Shader
{
public:
  virtual bool Load(const std::string&) { return true; }

  virtual void Begin() {}
  virtual void End() {}

  virtual void Set(const std::string& name, const float matrix[16]) {};
  virtual void Set(const std::string& name, float f) {};
  virtual void Set(const std::string& name, const AmjuGL::Vec3&) {};
  virtual void Set(const std::string& name, const Colour&) {};
  virtual void Set(const std::string& name, AmjuGL::TextureHandle) {};
};
}

#endif
