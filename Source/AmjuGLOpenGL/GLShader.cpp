//#ifndef IPHONE // TODO TEMP TEST

#include "AmjuFirst.h"
#include "GLShader.h"

#include <iostream>
#include "AmjuGL-OpenGL-Base.h"
#include "AmjuAssert.h"
#include "OpenGL.h"
#include <File.h>
#include "AmjuFinal.h"

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
  glGetShaderiv(m_programHandle, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    glGetShaderInfoLog(m_programHandle, 2000, 0, buf);
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

void GLShader::Begin()
{
  AMJU_CALL_STACK;

  glUseProgram(m_programHandle);
}

void GLShader::End()
{
  AMJU_CALL_STACK;

  glUseProgram(0);
}

void GLShader::Set(const std::string& name, const float matrix[16])
{
    GLint loc = glGetUniformLocation(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniformMatrix4fv(loc, 16, false, matrix);
}

void GLShader::Set(const std::string& name, float f)
{
    GLint loc = glGetUniformLocation(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform1f(loc, f);
}

void GLShader::Set(const std::string& name, const AmjuGL::Vec3& v)
{
    GLint loc = glGetUniformLocation(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform3f(loc, v.m_x, v.m_y, v.m_z);
}

void GLShader::Set(const std::string& name, const Colour& c)
{
    GLint loc = glGetUniformLocation(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform4fv(loc, 4, (float*)(&c));
}

void GLShader::Set(const std::string& name, AmjuGL::TextureHandle)
{
}
}

//#endif //  IPHONE // TODO TEMP TEST

