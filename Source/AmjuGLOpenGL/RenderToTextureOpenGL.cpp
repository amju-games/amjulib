#include <iostream>
#include <AmjuGL.h>
#include "Internal/OpenGL.h"
#include "RenderToTextureOpenGL.h"

// Thanks: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

namespace Amju
{
RenderToTextureOpenGL::RenderToTextureOpenGL()
{
  m_tex = -1;
  m_old_fbo = -1;
  m_framebuffer = -1;
  m_width = -1;
  m_height = -1;
}
  
RenderToTextureOpenGL::~RenderToTextureOpenGL()
{
  // TODO clean up
}
 
void RenderToTextureOpenGL::UseThisTexture()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_TEXTURE_2D);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  
  glBindTexture(GL_TEXTURE_2D, m_tex);
}
  
bool RenderToTextureOpenGL::Init()
{
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo);  
  
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  // create the texture
  glGenTextures(1, &m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);

  // Give an empty image to OpenGL ( the last "0" )
//  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  GLuint depthRenderbuffer;
  glGenRenderbuffers(1, &depthRenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);
  GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) 
  {
    std::cout << status << std::endl; // this is not called
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo);

  return true;
}

bool RenderToTextureOpenGL::Begin()
{
  AmjuGL::GetViewport(&m_vpX, &m_vpY, &m_vpW, &m_vpH);
  AmjuGL::Viewport(0, 0, m_width, m_height); // same as size in Init
    
  glBindTexture(GL_TEXTURE_2D, 0);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // depth??
  
  return true;
}

bool RenderToTextureOpenGL::End()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo);
  AmjuGL::Viewport(m_vpX, m_vpY, m_vpW, m_vpH);
  
  return true;
}
}

