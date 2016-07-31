#ifndef SHADOW_MAP_OPENGL_2_H
#define SHADOW_MAP_OPENGL_2_H

#include "ShadowMap.h"

namespace Amju
{
// Implement shadow map using OpenGL
class ShadowMapOpenGL2 : public ShadowMap
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

#endif

