#include <iostream>
#include <AmjuGL.h>
#include <AmjuGL-OpenGL-Base.h>
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
      GL_CHECK(glDeleteTextures(1, &m_tex[i]));
    }
  }
}
 
void RenderToTextureOpenGL::UseThisTexture()
{
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

#if !defined(AMJU_USE_ES2)  
  GL_CHECK(glEnable(GL_TEXTURE_2D));
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
#endif

  if (m_renderFlags == AMJU_RENDER_DEPTH)
  {
    // Only 1 texture, so use texture0, right?
    //GL_CHECK(glActiveTexture(GL_TEXTURE1));
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));
  }
  else if (m_renderFlags == AMJU_RENDER_COLOUR)
  {
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));
  }
  else
  {
    // Depth and colour buffers
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));

    GL_CHECK(glActiveTexture(GL_TEXTURE1));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
  }
}
  
bool RenderToTextureOpenGL::InitDepth()
{
  // Render depth buffer to texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));

  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
    m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0));

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

  // we won't bind a color texture with the currently binded FBO
#ifndef AMJU_USE_ES2
  GL_CHECK(glDrawBuffer(GL_NONE));
  GL_CHECK(glReadBuffer(GL_NONE));
#endif
  
  // attach the texture to FBO depth attachment point
  GL_CHECK(glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_tex[1], 0));

  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D, m_tex[1], 0));
  return true;
}

bool RenderToTextureOpenGL::InitDepthColour()
{

  for (int i = 0; i < 2; i++)
  {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[i]));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  }

  // parameters for color buffer texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, 0));

  // parameters for depth texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0));
    
  // attach the texture to FBO color attachment point
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, m_tex[0], 0));
    
  // Attach the texture to FBO depth attachment point  
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_TEXTURE_2D, m_tex[1], 0));

  return true;
}

bool RenderToTextureOpenGL::InitColour()
{
  // Render colour buffer to texture (with depth buffering)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));

  // Give an empty image to OpenGL ( the last "0" )
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    m_width, m_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL));

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLuint depthRenderbuffer;
  GL_CHECK(glGenRenderbuffers(1, &depthRenderbuffer));
  GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer));
  GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height));
  GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    GL_RENDERBUFFER, depthRenderbuffer));

  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex[0], 0));

  return true;
}

bool RenderToTextureOpenGL::Init()
{
  GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo));
  
  GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));

  // create the texture
  GL_CHECK(glGenTextures(2, m_tex));

/*
  for (int i = 0; i < 2; i++)
  {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[i]));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  }
*/

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
  
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo));

  return true;
}

bool RenderToTextureOpenGL::Begin()
{
  AmjuGL::GetViewport(&m_vpX, &m_vpY, &m_vpW, &m_vpH);
  AmjuGL::Viewport(0, 0, m_width, m_height); // same as size in Init
  
#if !defined(AMJU_USE_ES2)  
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
#endif

  GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo));
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));

  const Colour& c = AmjuGL::GetClearColour();
  GL_CHECK(glClearColor(c.m_r, c.m_g, c.m_b, 1));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  
  return true;
}

bool RenderToTextureOpenGL::End()
{
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo));
  AmjuGL::Viewport(m_vpX, m_vpY, m_vpW, m_vpH);
  
  return true;
}
}

