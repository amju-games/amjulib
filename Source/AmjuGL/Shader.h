#ifndef AMJU_SHADER_H
#define AMJU_SHADER_H

#include "AmjuGL.h"
#include "Colour.h"
#include "RCPtr.h"
#include "ResourceManager.h"

namespace Amju
{
class Shader;

// Base class for shader variables, i.e. uniforms & attributes
// Subclasses set the value of a uniform or attrib variable
class ShaderVariable : public RefCounted
{
public:
  ShaderVariable() : m_location(-1), m_shader(nullptr) {}

  void SetShader(Shader* sh) { m_shader = sh; }
  void SetName(const std::string& name) { m_name = name; }
  virtual void FindLocation() = 0;
  
protected:
  std::string m_name; // text name in shader
  int m_location;
  Shader* m_shader;
};

// Base class for uniform variables
class Uniform : public ShaderVariable
{
public:
  virtual void FindLocation() override;

  // Override this to set the value of the uniform
  virtual void SetUniform() = 0;
};

// Base class for attribute variables
class Attribute : public ShaderVariable
{
public:
  virtual void FindLocation() override;

  // Override this to set the value of the attrib
  virtual void SetAttrib() = 0;
};

// Factories for creating Uniforms and Attribs 
typedef Singleton<Factory<Uniform> > TheUniformFactory;
typedef Singleton<Factory<Attribute> > TheAttribFactory;

// Convenience templates to make Creation functions for above factories
template<class T> Uniform* CreateUniform() { return new T; }
template<class T> Attribute* CreateAttribute() { return new T; }


class Shader : public Resource
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

  // TODO Allow setting with location to avoid lookups
/*
  virtual void SetUniformMatrix(int uniformLocation, const float matrix[16]) = 0;
  virtual void SetUniformFloat(int uniformLocation, float f) = 0;
  // etc
*/

  // Do API-specific code to enable this shader. Call SetVariables to set
  //  values for uniforms and attribs.
  virtual void UseThisShader() = 0;
 
  void AddAttribute(Attribute* av);
  void AddUniform(Uniform* uv); 
  void SetVariables();

  // Do API-specific look up to find location of shader variables
  virtual int FindUniformLocation(const std::string& uniformName) = 0;
  virtual int FindAttribLocation(const std::string& attribName) = 0;

protected:
  typedef std::vector<Attribute*> AttributeVec;
  typedef std::vector<Uniform*> UniformVec;

  AttributeVec m_attribs;
  UniformVec m_uniforms;
};
}

#endif
