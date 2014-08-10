#if defined(AMJU_IOS) || defined (ANDROID_NDK)

#include <iostream>
#include <AmjuGL.h>
#include "RenderToTextureES2.h"
#include "AmjuGL-OpenGL-Base.h"

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
//  GL_CHECK(glEnable(GL_TEXTURE_2D));
//  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  // ??
//////  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  
  GL_CHECK(glActiveTexture(GL_TEXTURE0));  
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex));
}
  
bool RenderToTextureES2::Init()
{
  GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo));
  
#ifdef AMJU_IOS

  GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));

  // create the texture
  GL_CHECK(glGenTextures(1, &m_tex));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
  
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0));

  GLuint depthRenderbuffer = 0;
  GL_CHECK(glGenRenderbuffers(1, &depthRenderbuffer));
  GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer));
  GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height));
  GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer));
  
  GL_CHECK(glClearColor(0, 0, 0, 1));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // + depth??
  
#else
  
  GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
  GL_CHECK(glGenTextures(1, &m_tex));

  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex));
  
  // This fixed black screen of death
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               width, height, 0, GL_RGBA, 
               GL_UNSIGNED_BYTE, 0));
  
  // Use GL_COLOR_ATTACHMENT0 or GL_DEPTH_ATTACHMENT????
  
  // Doesn't work
//  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tex, 0);
  
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0));

  GL_CHECK(glClearColor(0, 0, 0, 1));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT)); // + depth??

#endif
  
  GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) 
  {
    std::cout << status << std::endl; // this is not called
  }
  
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo));

  return true;
}

bool RenderToTextureES2::Begin()
{
  AmjuGL::GetViewport(&m_vpX, &m_vpY, &m_vpW, &m_vpH);
  AmjuGL::Viewport(0, 0, m_width, m_height); // same as size in Init
    
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo));
//  std::cout << "Wow, old fbo is " << m_old_fbo << "\n";
  
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));

  const Colour& c = AmjuGL::GetClearColour();
  GL_CHECK(glClearColor(c.m_r, c.m_g, c.m_b, 1));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  
  return true;
}

bool RenderToTextureES2::End()
{
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo));
  AmjuGL::Viewport(m_vpX, m_vpY, m_vpW, m_vpH);
  
  return true;
}
}

#endif


