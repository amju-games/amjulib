#if defined(AMJU_IOS) || defined (ANDROID_NDK)

#include <iostream>
#include <AmjuGL.h>
#include "RenderToTextureES2.h"

namespace Amju
{
RenderToTextureES2::RenderToTextureES2()
{
  // TODO intialise
  m_tex = -1;
  m_old_fbo = -1;
  m_framebuffer = -1;
  m_width = 512;
  m_height = 512;
}
  
RenderToTextureES2::~RenderToTextureES2()
{
  // TODO clean up
}
 
void RenderToTextureES2::UseThisTexture()
{
  glBindTexture(GL_TEXTURE_2D, m_tex);
}
  
bool RenderToTextureES2::Init(int w, int h)
{
  m_width = w;
  m_height = h;
  
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo);
  
  
#ifdef AMJU_IOS

  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  // create the texture
  glGenTextures(1, &m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

  GLuint depthRenderbuffer;
  glGenRenderbuffers(1, &depthRenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
  
#else
  
  glGenFramebuffers(1, &m_framebuffer);
  glGenTextures(1, &m_tex);

  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
  glBindTexture(GL_TEXTURE_2D, m_tex);
  
  // This fixed black screen of death
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
               width, height, 0, GL_RGBA, 
               GL_UNSIGNED_BYTE, 0);
  
  // Use GL_COLOR_ATTACHMENT0 or GL_DEPTH_ATTACHMENT????
  
  // Doesn't work
//  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tex, 0);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT); // + depth??

#endif
  
  GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) 
  {
    std::cout << status << std::endl; // this is not called
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo);

  return true;
}

static int x, y, w, h; // remember viewport

bool RenderToTextureES2::Begin()
{
  AmjuGL::GetViewport(&x, &y, &w, &h);
  AmjuGL::Viewport(0, 0, m_width, m_height); // same as size in Init
    
  glBindTexture(GL_TEXTURE_2D, 0);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // depth??
  
  return true;
}

bool RenderToTextureES2::End()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo);
  AmjuGL::Viewport(x, y, w, h); 
  
  return true;
}
}

#endif


