#include <iostream>
#include <AmjuGL.h>
#include "Internal/OpenGL.h"
#include "RenderToTextureOpenGL.h"

// Thanks: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

namespace Amju
{
RenderToTextureOpenGL::RenderToTextureOpenGL()
{
  m_tex[0] = -1;
  m_tex[1] = -1;
  m_old_fbo = -1;
  m_framebuffer = -1;
  m_width = -1;
  m_height = -1;
}
  
RenderToTextureOpenGL::~RenderToTextureOpenGL()
{
  // TODO clean up framebuffer etc

  for (int i = 0; i < 2; i++)
  {
    if (m_tex[i] > 0)
    {
      glDeleteTextures(1, &m_tex[i]);
    }
  }
}
 
void RenderToTextureOpenGL::UseThisTexture()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_TEXTURE_2D);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  
  if (m_renderFlags == AMJU_RENDER_DEPTH)
  {
    glBindTexture(GL_TEXTURE_2D, m_tex[1]);
  }
  else if (m_renderFlags == AMJU_RENDER_COLOUR)
  {
    glBindTexture(GL_TEXTURE_2D, m_tex[0]);
  }
  else
  {
    // Depth and colour buffers
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_tex[1]);

    glActiveTexture(GL_TEXTURE0);
  }
}
  
bool RenderToTextureOpenGL::InitDepth()
{
  // Render depth buffer to texture
  glBindTexture(GL_TEXTURE_2D, m_tex[1]);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
    m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

  glBindTexture(GL_TEXTURE_2D, 0);

  // we won't bind a color texture with the currently binded FBO
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // attach the texture to FBO depth attachment point
  glFramebufferTexture2DEXT(
    GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, m_tex[1], 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex[1], 0);
  return true;
}

bool RenderToTextureOpenGL::InitDepthColour()
{
  // parameters for color buffer texture
  glBindTexture(GL_TEXTURE_2D, m_tex[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, 0);

  // parameters for depth texture
  glBindTexture(GL_TEXTURE_2D, m_tex[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, 
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    
  // attach the texture to FBO color attachment point
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, m_tex[0], 0);
    
  // Attach the texture to FBO depth attachment point  
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                              GL_TEXTURE_2D, m_tex[1], 0);

  return true;
}

bool RenderToTextureOpenGL::InitColour()
{
  // Render colour buffer to texture (with depth buffering)
  glBindTexture(GL_TEXTURE_2D, m_tex[0]);

  // Give an empty image to OpenGL ( the last "0" )
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
    m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  GLuint depthRenderbuffer;
  glGenRenderbuffers(1, &depthRenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
    GL_RENDERBUFFER, depthRenderbuffer);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex[0], 0);

  return true;
}

bool RenderToTextureOpenGL::Init()
{
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo);  
  
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  // create the texture
  glGenTextures(2, m_tex);

  for (int i = 0; i < 2; i++)
  {
    glBindTexture(GL_TEXTURE_2D, m_tex[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  if (m_renderFlags == AMJU_RENDER_DEPTH)
  { 
    InitDepth();
  }
  else if (m_renderFlags == AMJU_RENDER_COLOUR)
  {
    InitColour();
  }
  else
  {
    InitDepthColour();
  }
  
  GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) 
  {
    std::cout << status << std::endl; 
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

  const Colour& c = AmjuGL::GetClearColour();
  glClearColor(c.m_r, c.m_g, c.m_b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
  return true;
}

bool RenderToTextureOpenGL::End()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo);
  AmjuGL::Viewport(m_vpX, m_vpY, m_vpW, m_vpH);
  
  return true;
}
}

