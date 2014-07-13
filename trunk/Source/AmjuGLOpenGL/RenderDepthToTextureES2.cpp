#include <iostream>
#include <AmjuGL.h>
#include "RenderToTextureES2.h"

namespace Amju
{
bool Init(int w, int h)
{
  // Create a framebuffer object
  glGenFramebuffers(1, fboId);
  glBindFramebuffer(GL_FRAMEBUFFER, *fboId);

  // Create a render buffer
  glGenRenderbuffers(1, renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, *renderBuffer);
	 
  // Create a texture for storing the depth
  glGenTextures(1, depthTextureId);
  glBindTexture(GL_TEXTURE_2D, *depthTextureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Remove artifact on the edges of the shadowmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  Debug("glGetError() 0 = 0x%x", glGetError());
  if ( m_hasDepthTextureExtension )
  {
      // We'll use a depth texture to store the depths in the shadow map
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

      // Attach the depth texture to FBO depth attachment point
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_2D, *depthTextureId, 0);
  }
  else
  {
      // We'll use a RGBA texture into which we pack the depth info
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, NULL);

      // Attach the RGBA texture to FBO color attachment point
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D, *depthTextureId, 0);

      // Allocate 16-bit depth buffer
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

      // Attach the render buffer as depth buffer - will be ignored
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, *renderBuffer);
  }

  // check FBO status
  GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if ( fboStatus != GL_FRAMEBUFFER_COMPLETE )
  {
      Debug("FBO not complete: 0x%x", fboStatus);
      return false;
  }

  // Go back to using the default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, DefaultFramebufferId);

  return true;
}


}

