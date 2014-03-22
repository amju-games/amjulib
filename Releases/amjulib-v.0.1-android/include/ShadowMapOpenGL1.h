#ifndef SHADOW_MAP_OPENGL_1_H
#define SHADOW_MAP_OPENGL_1_H

#include "ShadowMap.h"

namespace Amju
{
// Implement shadow map using OpenGL
class ShadowMapOpenGL1 : public ShadowMap
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

#endif

