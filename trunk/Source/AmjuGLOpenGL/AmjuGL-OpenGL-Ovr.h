/*
Amju Games source code (c) Copyright Jason Colman 2014
*/

#pragma once

#include "AmjuGL-OpenGL.h"

namespace Amju
{
class AmjuGLOpenGL_Ovr : public AmjuGLOpenGL
{
public:
  // Call once at app startup
  virtual void Init();

  // Call to create window
  virtual bool CreateWindow(AmjuGLWindowInfo*);

  // Call when all drawing finished
  virtual void Flip();

};
}

