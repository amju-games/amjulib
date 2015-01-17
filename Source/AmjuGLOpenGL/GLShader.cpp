#include <AmjuFirst.h>
#if (!defined(AMJU_IOS) && !defined(ANDROID_NDK)) || defined(AMJU_USE_ES2)

#include "GLShader.h"

#include <iostream>
#include "AmjuGL-OpenGL-Base.h"
#include "AmjuAssert.h"
#include "Internal/OpenGL.h"
#include <File.h>
#include <ReportError.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

#define SHADER_DEBUG

namespace Amju
{
GLShader::GLShader()
{
  AMJU_CALL_STACK;

  m_vertexShaderHandle = 0;
  m_fragmentShaderHandle = 0;
  m_programHandle = 0;
} 

GLShader::~GLShader()
{
  AMJU_CALL_STACK;
  if (m_programHandle)
  {
    glDeleteProgram(m_programHandle);
  }
}

int GLShader::GetAttribLocation(const std::string& attribName)
{
  Assert(m_programHandle);
  
  LocationMap::iterator it = m_map.find(attribName);
  if (it != m_map.end())
  {
    return it->second;
  }
  int loc = (int)glGetAttribLocation(m_programHandle, attribName.c_str());
  m_map[attribName] = loc;
  return loc;
}
  
int GLShader::GetUniformLocation(const std::string& uniformName)
{
  Assert(m_programHandle);

  LocationMap::iterator it = m_map.find(uniformName);
  if (it != m_map.end())
  {
    return it->second;
  }
  int loc = (int)glGetUniformLocation(m_programHandle, uniformName.c_str());
  m_map[uniformName] = loc;
  return loc;
}

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

std::string ToString(const FileLineVec& vec)
{
  std::string res;
  for (int i = 0; i < vec.size(); i++)
  {
    const FileLine& fl = vec[i];
    res += fl.m_text + "\n";
  }
  return res;
}

bool LoadFileWithIncludes(const std::string& path, const std::string& filename, FileLineVec* result)
{
  File file(File::NO_VERSION);
  if (!file.OpenRead(path + filename))
  {
    std::string badInclude;
    if (!result->empty())
    {
      const FileLine& fl = result->back();
      badInclude = " (included from " + fl.m_filename + ": " + ToString(fl.m_lineNum) + ")";
    }
    file.ReportError("Failed to open file" + badInclude);
    return false;
  }
  
  std::string s;
  const bool NO_TRIM = false;
  int lineNum = 1; // line numbers start at one
  while (file.GetDataLine(&s, NO_TRIM))
  {
    if (StringContains(s, "#include"))
    {
      const std::string includefilename = s.substr(9);
std::cout << "Including file \"" << includefilename << "\"\n";
      bool ok = LoadFileWithIncludes(path, includefilename, result);
      if (!ok)
      {
        return false;
      }
    }
    else
    {
      result->push_back(FileLine(s, filename, lineNum));
    }
    lineNum++;
  }
  return true;
}

bool LoadShaderFile(const std::string filename, FileLineVec* vec)
{
  return LoadFileWithIncludes(GetFilePath(filename) + "/", StripPath(filename), vec);
}

bool GLShader::Load(const std::string& shadername)
{
#ifdef SHADER_DEBUG
std::cout << "Loading shader: " << shadername << "\n";
#endif

  if (!AmjuGLOpenGLBase::s_shaderSupport)
  {
    m_errorStr = "GLSL not supported";
    ReportError("GLSL not supported\n");
    return false;
  }

  // Load files, pass strings to Create()
  // TODO No good, we want the file/line number info
  FileLineVec frag;
  if (!LoadShaderFile(shadername + "-frag.txt", &frag))
  {
    ReportError("Failed to load fragment shader file");
    return false;
  }
  std::string fragText = ToString(frag);

  FileLineVec vert;
  if (!LoadShaderFile(shadername + "-vert.txt", &vert))
  {
    ReportError("Failed to load vertex shader file");
    return false;
  }
  std::string vertText = ToString(vert);

  return Create(vertText, fragText);
}

bool GLShader::Create(const std::string& vertexSource, const std::string& fragmentSource)
{
  AMJU_CALL_STACK;

  if (!AmjuGLOpenGLBase::s_shaderSupport)
  {
    m_errorStr = "GLSL not supported";
    return false;
  }

#ifdef SHADER_DEBUG
std::cout << "Vertex Shader source:\n" << vertexSource.c_str()
  << "\nFragment Shader source:\n" << fragmentSource.c_str() << "\n";
#endif

  m_vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
  m_fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

#ifdef SHADER_DEBUG
std::cout << "Created shader handles\n";
#endif

  const GLint vlength = vertexSource.size();
  const GLint flength = fragmentSource.size();
  const char* vStr = vertexSource.c_str();
  const char* fStr = fragmentSource.c_str();
  glShaderSource(m_vertexShaderHandle, 1, &vStr, &vlength);
  glShaderSource(m_fragmentShaderHandle, 1, &fStr, &flength);

#ifdef SHADER_DEBUG
std::cout << "Set shader source\n";
#endif

  GLint compiled = 0;
  GLchar buf[2000]; // error string buffer

  glCompileShader(m_vertexShaderHandle);

  glGetShaderiv(m_vertexShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetShaderInfoLog(m_vertexShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

#ifdef SHADER_DEBUG
std::cout << "Vertex Shader Compile error: " << buf << "\n";
#endif
    return false;
  }
#ifdef SHADER_DEBUG
std::cout << "Compiled vertex shader\n";
#endif

  glCompileShader(m_fragmentShaderHandle);
  
  glGetShaderiv(m_fragmentShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetShaderInfoLog(m_fragmentShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

#ifdef SHADER_DEBUG
std::cout << "Fragment Shader Compile error: " << buf << "\n";
#endif
    return false;
  }        

#ifdef SHADER_DEBUG
std::cout << "Compiled fragment shader\n";
#endif

  m_programHandle = glCreateProgram();

#ifdef SHADER_DEBUG
std::cout << "Created program\n";
#endif

  glAttachShader(m_programHandle, m_vertexShaderHandle);
  glAttachShader(m_programHandle, m_fragmentShaderHandle);

#ifdef SHADER_DEBUG
std::cout << "Attached shaders to program\n";
#endif

  glLinkProgram(m_programHandle);

#ifdef SHADER_DEBUG
std::cout << "Link called\n";
#endif

  GLint linked;
  glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    glGetProgramInfoLog(m_programHandle, 2000, 0, buf);
    m_errorStr = buf;
    
#ifdef SHADER_DEBUG
std::cout << "Shader Link error: " << buf << "\n";
#endif
    return false; 
  }        

#ifdef SHADER_DEBUG
std::cout << "Shader program linked OK!\n";
#endif

  return true;
}

std::string GLShader::GetErrorString()
{
  return m_errorStr; 
}

static uint32 prevHandle = 0;

void GLShader::Begin()
{
  AMJU_CALL_STACK;

  // Don't call glUseProgram if we are already using it
  // - not reliable
  //if (prevHandle != m_programHandle)
  {
    //std::cout << "Using shader program " << m_programHandle << "\n";
    
    AmjuGL::UseShader(this);
  }
}

void GLShader::UseThisShader()
{
  glUseProgram(m_programHandle);
  prevHandle = m_programHandle;
}

void GLShader::End()
{
  AMJU_CALL_STACK;

  prevHandle = 0;
  AmjuGL::UseShader(0);
  glUseProgram(0);
}
  
void GLShader::SetMatrix3x3(const std::string& name, const float matrix[9])
{
  GLint loc = GetUniformLocation(name);
  
  if (loc == -1)
  {
    ReportError("GLShader: didn't find uniform var " + name + " in shader");
    Assert(0);
    return;
  }
  
  glUniformMatrix3fv(loc, 1, false, matrix);
}

void GLShader::Set(const std::string& name, const float matrix[16])
{
    GLint loc = GetUniformLocation(name); 

    if (loc == -1)
    {
        ReportError("GLShader: didn't find uniform var " + name + " in shader");
        Assert(0);
        return;
    }

    glUniformMatrix4fv(loc, 1, false, matrix);
}

void GLShader::Set(const std::string& name, float f)
{
    GLint loc = GetUniformLocation(name); 
      //glGetUniformLocation(m_programHandle, name.c_str());

    if (loc == -1)
    {
        ReportError("GLShader: didn't find uniform var " + name + " in shader");
        Assert(0);
    }

    glUniform1f(loc, f);
}

void GLShader::Set(const std::string& name, const AmjuGL::Vec3& v)
{
    GLint loc = GetUniformLocation(name); 
      //glGetUniformLocation(m_programHandle, name.c_str());

    if (loc == -1)
    {
        ReportError("GLShader: didn't find uniform var " + name + " in shader");
        Assert(0);
        return;
    }

    glUniform3f(loc, v.m_x, v.m_y, v.m_z);
}

void GLShader::Set(const std::string& name, const Colour& c)
{
    GLint loc = GetUniformLocation(name); 
      //glGetUniformLocation(m_programHandle, name.c_str());

    if (loc == -1)
    {
        ReportError("GLShader: didn't find uniform var " + name + " in shader");
        Assert(0);
        return;
    }

    glUniform4f(loc, c.m_r, c.m_g, c.m_b, c.m_a);
}

void GLShader::Set(const std::string& name, AmjuGL::TextureHandle t)
{
  GLint loc = GetUniformLocation(name); 

  if (loc == -1)
  {
    ReportError("GLShader: didn't find uniform var " + name + " in shader");
    Assert(0);
    return;
  }
  
  glUniform1i(loc, (int)t);
}
  
void GLShader::SetInt(const std::string& name, int i)
{
  GLint loc = GetUniformLocation(name);
  
  if (loc == -1)
  {
    ReportError("GLShader: didn't find uniform var " + name + " in shader");
    Assert(0);
    return;
  }
  
  glUniform1i(loc, i);
}
  
int GLShader::GetProgHandle() const
{
  return m_programHandle;
}
  
}

#endif //  !IPHONE || AMJU_USE_ES2

