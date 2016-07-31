#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <map>
#include <string>
#include "Shader.h"

namespace Amju
{
// Recursive file load, allowing for #includes
// -------------------------------------------
struct FileLine
{ 
  FileLine() : m_lineNum(-1) {}
  FileLine(const std::string& text, const std::string& filename, int lineNum) : 
    m_text(text), m_filename(filename), m_lineNum(lineNum) {}
 
  std::string m_text;
  std::string m_filename;
  int m_lineNum;
};
typedef std::vector<FileLine> FileLineVec;

class GLShader : public Shader
{
public:
  GLShader();
  ~GLShader();

  virtual bool Load(const std::string& filenamePrefix);
  
  bool Create(const std::string& vertexSource, const std::string& fragmentSource);

  std::string GetErrorString();

  // Start using the shader
  void Begin() override;
  // Finish using the shader
  void End() override;

  virtual void SetMatrix3x3(const std::string& name, const float matrix[9]) override;
  virtual void Set(const std::string& name, const float matrix[16]) override;
  virtual void Set(const std::string& name, float f) override;
  virtual void Set(const std::string& name, const AmjuGL::Vec3&) override;
  virtual void Set(const std::string& name, const Colour&) override;
  virtual void Set(const std::string& name, AmjuGL::TextureHandle) override;
  virtual void SetInt(const std::string& name, int i) override;

  virtual void UseThisShader() override;
  
  int GetProgHandle() const;
  
  int FindUniformLocation(const std::string& uniformName) override;
  int FindAttribLocation(const std::string& attribName) override;

private:
  bool LoadShaderFile(const std::string filename, FileLineVec* vec);
  bool LoadFileWithIncludes(const std::string& path, const std::string& filename, FileLineVec* result);

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
