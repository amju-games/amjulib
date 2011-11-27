#ifndef SHADOW_MAP_OPENGL_3_H
#define SHADOW_MAP_OPENGL_3_H

#include <ShadowMap.h>

namespace Amju
{
// Implement shadow map using OpenGL
class ShadowMapOpenGL3 : public ShadowMap
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

#endif

