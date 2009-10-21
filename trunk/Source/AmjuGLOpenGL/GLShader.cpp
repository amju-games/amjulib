#include "AmjuFirst.h"
#include "GLShader.h"
//#ifdef WIN32
//#include <windows.h>
//#endif

#include <iostream>
#include "GL/glew.h"
#include "AmjuGL-OpenGL.h"
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
  if (!AmjuGLOpenGL::s_shaderSupport)
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
  while (fragFile.GetDataLine(&s))
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
  while (vertFile.GetDataLine(&s))
  {
    vertText += s;
    vertText += "\n";
  }

  return Create(vertText, fragText);
}

bool GLShader::Create(const std::string& vertexSource, const std::string& fragmentSource)
{
  AMJU_CALL_STACK;

  if (!AmjuGLOpenGL::s_shaderSupport)
  {
    m_errorStr = "GLSL not supported";
    return false;
  }

#ifdef SHADER_DEBUG
std::cout << "Loading shader..\nVertex Shader source:\n" << vertexSource.c_str()
  << "\nFragment Shader source:\n" << fragmentSource.c_str() << "\n";
#endif

  m_vertexShaderHandle = glCreateShaderObjectARB(GL_VERTEX_SHADER);
  m_fragmentShaderHandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

#ifdef SHADER_DEBUG
std::cout << "Created shader handles\n";
#endif

  const GLint vlength = vertexSource.size();
  const GLint flength = fragmentSource.size();
  const char* vStr = vertexSource.c_str();
  const char* fStr = fragmentSource.c_str();
  glShaderSourceARB(m_vertexShaderHandle, 1, &vStr, &vlength);
  glShaderSourceARB(m_fragmentShaderHandle, 1, &fStr, &flength);

#ifdef SHADER_DEBUG
std::cout << "Set shader source\n";
#endif

  GLint compiled = 0;
  GLcharARB buf[2000]; // error string buffer

  glCompileShaderARB(m_vertexShaderHandle);

  glGetObjectParameterivARB(m_vertexShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetInfoLogARB(m_vertexShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

#ifdef SHADER_DEBUG
std::cout << "Vertex Shader Compile error: " << buf << "\n";
#endif
    return false;
  }
#ifdef SHADER_DEBUG
std::cout << "Compiled vertex shader\n";
#endif

  glCompileShaderARB(m_fragmentShaderHandle);
  
  glGetObjectParameterivARB(m_fragmentShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetInfoLogARB(m_fragmentShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

#ifdef SHADER_DEBUG
std::cout << "Fragment Shader Compile error: " << buf << "\n";
#endif
    return false;
  }        

#ifdef SHADER_DEBUG
std::cout << "Compiled fragment shader\n";
#endif

  m_programHandle = glCreateProgramObjectARB();

#ifdef SHADER_DEBUG
std::cout << "Created program\n";
#endif

  glAttachObjectARB(m_programHandle, m_vertexShaderHandle);
  glAttachObjectARB(m_programHandle, m_fragmentShaderHandle);

#ifdef SHADER_DEBUG
std::cout << "Attached shaders to program\n";
#endif

  glLinkProgramARB(m_programHandle);

#ifdef SHADER_DEBUG
std::cout << "Link called\n";
#endif

  GLint linked;
  glGetObjectParameterivARB(m_programHandle, GL_OBJECT_LINK_STATUS_ARB, &linked);
  if (!linked)
  {
    glGetInfoLogARB(m_programHandle, 2000, 0, buf);
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

  glUseProgramObjectARB(m_programHandle);
}

void GLShader::End()
{
  AMJU_CALL_STACK;

  glUseProgramObjectARB(0);
}

void GLShader::Set(const std::string& name, const float matrix[16])
{
    GLint loc = glGetUniformLocationARB(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniformMatrix4fvARB(loc, 16, false, matrix);
}

void GLShader::Set(const std::string& name, float f)
{
    GLint loc = glGetUniformLocationARB(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform1fARB(loc, f);
}

void GLShader::Set(const std::string& name, const AmjuGL::Vec3& v)
{
    GLint loc = glGetUniformLocationARB(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform3fARB(loc, v.m_x, v.m_y, v.m_z);
}

void GLShader::Set(const std::string& name, const Colour& c)
{
    GLint loc = glGetUniformLocationARB(m_programHandle, name.c_str());
    if (loc == -1)
    {
        return;
    }

    glUniform4fvARB(loc, 4, (float*)(&c));
}

void GLShader::Set(const std::string& name, AmjuGL::TextureHandle)
{
}
}
