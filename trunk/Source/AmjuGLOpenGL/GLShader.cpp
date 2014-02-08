#include <AmjuFirst.h>
#if !defined(IPHONE) || defined(AMJU_USE_ES2)

#include "GLShader.h"

#include <iostream>
#include "AmjuGL-OpenGL-Base.h"
#include "AmjuAssert.h"
#include "Internal/OpenGL.h"
#include <File.h>
#include <ReportError.h>
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

bool GLShader::Load(const std::string& shadername)
{
#ifdef SHADER_DEBUG
std::cout << "Loading shader: " << shadername << "\n";
#endif

  if (!AmjuGLOpenGLBase::s_shaderSupport)
  {
    m_errorStr = "GLSL not supported";
    return false;
  }

  // TODO Load files, pass strings to Create()
  File fragFile(File::NO_VERSION);
  if (!fragFile.OpenRead(shadername + "-frag.txt"))
  {
    fragFile.ReportError("Failed to open fragment shader file");
    return false;
  }
  std::string fragText;
  std::string s;
  const bool NO_TRIM = false;
  while (fragFile.GetDataLine(&s, NO_TRIM))
  {
    fragText += s;
    fragText += "\n";
  }

  File vertFile(File::NO_VERSION);
  if (!vertFile.OpenRead(shadername + "-vert.txt"))
  {
    vertFile.ReportError("Failed to open vertex shader file");
    return false;
  }
  std::string vertText;
  while (vertFile.GetDataLine(&s, NO_TRIM))
  {
    vertText += s;
    vertText += "\n";
  }

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
  if (prevHandle != m_programHandle)
  {
    glUseProgram(m_programHandle);
    prevHandle = m_programHandle;
  }
}

void GLShader::End()
{
  AMJU_CALL_STACK;

  prevHandle = 0;
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

