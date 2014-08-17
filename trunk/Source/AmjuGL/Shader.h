#ifndef AMJU_SHADER_H
#define AMJU_SHADER_H

#include "AmjuGL.h"
#include "Colour.h"
#include "RCPtr.h"

namespace Amju
{
class Shader : public RefCounted
{
public:
  virtual ~Shader() {}

  // TODO Allow multiple techniques ?
  virtual void Begin() = 0;
  virtual void End() = 0;

  // ? Allow multipass effects ?
  /*
  int GetNumPasses() const;
  void BeginPass(int passNum);
  void EndPass();
  */

  virtual void Set(const std::string& name, const float matrix[16]) = 0;
  virtual void Set(const std::string& name, float f) = 0 ;
  virtual void Set(const std::string& name, const AmjuGL::Vec3&) = 0;
  virtual void Set(const std::string& name, const Colour&) = 0;
  virtual void Set(const std::string& name, AmjuGL::TextureHandle) = 0;
  virtual void SetMatrix3x3(const std::string& name, const float matrix[9]) = 0;
  virtual void SetInt(const std::string& name, int i) = 0;
  
  virtual void UseThisShader() = 0;
  
};
}

#endif
