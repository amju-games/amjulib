/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include <AmjuFirst.h>
#include <iostream>
#include <Drawable.h>
#include "Internal/OpenGL.h"
#include "AmjuGL-OpenGL-Base.h"
#include <AmjuAssert.h>
#include <AmjuFinal.h>

//#define SHADER_DEBUG
//#define OPENGL_SHOW_INFO

namespace Amju
{
void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
  {
    std::string str;
    if (err == GL_INVALID_ENUM) str = "GL_INVALID_ENUM";
    else if (err == GL_INVALID_VALUE) str = "GL_INVALID_VALUE";
    else if (err == GL_INVALID_OPERATION) str = "GL_INVALID_OPERATION";
	else if (err == GL_OUT_OF_MEMORY) str = "GL_OUT_OF_MEMORY";

#ifdef GL_STACK_OVERFLOW
    else if (err == GL_STACK_OVERFLOW) str = "GL_STACK_OVERFLOW";
#endif

#ifdef GL_STACK_UNDERFLOW
    else if (err == GL_STACK_UNDERFLOW) str = "GL_STACK_UNDERFLOW";
#endif

#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
    else if (err == GL_INVALID_FRAMEBUFFER_OPERATION) str = "GL_INVALID_FRAMEBUFFER_OPERATION";
#endif

#ifdef GL_TABLE_TOO_LARGE
    else if (err == GL_TABLE_TOO_LARGE) str = "GL_TABLE_TOO_LARGE";
#endif

    else str = "Unknown error!";

    std::cout << "OpenGL error " << str << " (" << err << ") at " << fname << ": " << line <<
      " for: " << stmt << "\n";
//    Assert(0);
  }
}
  
bool AmjuGLOpenGLBase::s_shaderSupport = false;

void AmjuGLOpenGLBase::BeginScene()
{
  AMJU_CALL_STACK;

}

void AmjuGLOpenGLBase::EndScene()
{
  AMJU_CALL_STACK;

}

void AmjuGLOpenGLBase::Init()
{
  AMJU_CALL_STACK;

  
#ifdef _DEBUG
  const unsigned char* version = glGetString(GL_VERSION);
  std::cout << "OpenGL Version: " << version << "\n";
#endif
  
#ifdef OPENGL_SHOW_INFO
  const unsigned char* vendor = glGetString(GL_VENDOR);
  const unsigned char* renderer = glGetString(GL_RENDERER);
  const unsigned char* extensions = glGetString(GL_EXTENSIONS);
  std::cout << "OpenGL Vendor: " << vendor << "\n";
  std::cout << "OpenGL Renderer: " << renderer << "\n";
  std::cout << "OpenGL Extensions: " << extensions << "\n";
#endif
  
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();

  GL_CHECK(glEnable(GL_CULL_FACE));
  GL_CHECK(glCullFace(GL_BACK));

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  //glEnable(GL_COLOR_MATERIAL);

  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  // Try to disable blending unless expicitly required
  AmjuGL::Disable(AmjuGL::AMJU_BLEND); // So flag is reset
  
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
}

void AmjuGLOpenGLBase::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  // Do GL initialisation before we draw the frame.
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  GL_CHECK(glClearColor(clearR, clearG, clearB, 1.0f));

  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  GL_CHECK(glCullFace(GL_BACK));
  GL_CHECK(glEnable(GL_CULL_FACE));
  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  AmjuGL::Disable(AmjuGL::AMJU_BLEND); // only enabled where necessary
  // NB Must use AmjuGL::Disable so flags are set consistently!

  //glEnable(GL_BLEND); // TODO some things (day night sky etc) must enable this.
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  // Set the modelview matrix
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
}

void AmjuGLOpenGLBase::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  GL_CHECK(glViewport(x, y, w, h));
}

void AmjuGLOpenGLBase::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

}

void AmjuGLOpenGLBase::PopAttrib()
{
  AMJU_CALL_STACK;

}

void AmjuGLOpenGLBase::DestroyTextureHandle(AmjuGL::TextureHandle* th)
{
  AMJU_CALL_STACK;

  int texId = (int)(*th);
  if (texId < 0)
  {
    std::cout << "\n*** Texture handle not found: " << texId << "\n";
    Assert(0);
  }

  GL_CHECK(glDeleteTextures(1, (GLuint*)th));
}

void AmjuGLOpenGLBase::UseTexture(AmjuGL::TextureHandle t)
{
  AMJU_CALL_STACK;

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, t));
}

void AmjuGLOpenGLBase::SetTextureMode(AmjuGL::TextureMode tm)
{
  if (tm == AmjuGL::AMJU_TEXTURE_WRAP)
  {
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  }
  else if (tm == AmjuGL::AMJU_TEXTURE_CLAMP)
  {
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  }
  else
  {
    Assert(0);
  }
}

void AmjuGLOpenGLBase::SetTextureFilter(AmjuGL::TextureFilter tf)
{
  if (tf == AmjuGL::AMJU_TEXTURE_NICE)
  {
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  }
  else if (tf == AmjuGL::AMJU_TEXTURE_NEAREST)
  {
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  }
  else
  {
    Assert(0);
  }
}

}



