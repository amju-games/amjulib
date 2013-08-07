#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <AmjuGL.h>
#include "DrawDecorator.h"

namespace Amju
{
// Render using a Shadow Map
class ShadowMap : public DrawDecorator
{
public:
  enum { DRAWABLE_TYPE_ID = 5 };
  void SetLightPos(const AmjuGL::Vec3& lp) { m_lightPos = lp; }

protected:
  AmjuGL::Vec3 m_lightPos;
};

typedef RCPtr<ShadowMap> PShadowMap;

class ShadowMapNull : public ShadowMap
{
public:
  virtual bool Init() { return true; }
  virtual void Draw() { m_drawFunc(); }
};
}

#endif

