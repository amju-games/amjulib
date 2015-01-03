#if defined (AMJU_IOS) || defined (ANDROID_NDK) || defined(AMJU_USE_ES2)

#include <iostream>
#include <AmjuGL.h>
#include "RenderToTextureES2.h"
#include "AmjuGL-OpenGL-Base.h"

namespace Amju
{
RenderToTextureES2::RenderToTextureES2()
{
  // TODO intialise
  m_tex[0] = m_tex[1] = -1;
  m_old_fbo = -1;
  m_framebuffer = -1;
  m_width = -1;
  m_height = -1;
}
  
RenderToTextureES2::~RenderToTextureES2()
{
  // TODO clean up
}
 
void RenderToTextureES2::UseThisTexture()
{
  if (m_renderFlags == AMJU_RENDER_DEPTH)
  {
    // Only 1 texture, so use texture0, right?
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
  
bool RenderToTextureES2::InitDepth()
{
  // Render depth buffer to texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));
    
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                        m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0));
    
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  
  // attach the texture to FBO depth attachment point
  GL_CHECK(glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_tex[1], 0));
  
  // DON'T do this
//  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                                    GL_TEXTURE_2D, m_tex[1], 0));
  return true;
}
  
bool RenderToTextureES2::InitColour()
{
  // Render colour buffer to texture (with depth buffering)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));
    
  // Give an empty image to OpenGL ( the last "0" )
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                          m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    
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
  
bool RenderToTextureES2::InitDepthColour()
{
  // parameters for color buffer texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[0]));

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, 0));
    
  // parameters for depth texture
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_tex[1]));
  
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  
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
  
bool RenderToTextureES2::Init()
{
  GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_old_fbo));
  
  GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));

  GL_CHECK(glGenTextures(2, m_tex));
  
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
    std::string str;
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) str = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) str = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    else if (status == GL_FRAMEBUFFER_UNSUPPORTED) str = "GL_FRAMEBUFFER_UNSUPPORTED";

#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) str = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER;
#endif
        
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) str = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER;
#endif
        
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) str = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE;
#endif
        
#ifdef GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS) str = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
#endif
      
#ifdef GL_FRAMEBUFFER_UNDEFINED
    else if (status == GL_FRAMEBUFFER_UNDEFINED) str = "GL_FRAMEBUFFER_UNDEFINED";
#endif
    
    else str = "Unknown error code!";
    
    std::cout << "Framebuffer error: " << status << ": " << str << std::endl;
    Assert(0);
  }
  
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_old_fbo));

  return true;
}

bool RenderToTextureES2::Begin()
{
  AmjuGL::GetViewport(&m_vpX, &m_vpY, &m_vpW, &m_vpH);
  AmjuGL::Viewport(0, 0, m_width, m_height); // same as size in Init
    
//  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
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


