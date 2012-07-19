#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <AmjuGL.h>
#include <RCPtr.h>

namespace Amju
{
// Render using a Shadow Map
class ShadowMap : public RefCounted
{
public:
  ShadowMap() : m_drawFunc(0) {}
  virtual ~ShadowMap() {}

  typedef void (*DrawFunc)();
  void SetDrawFunc(DrawFunc df) { m_drawFunc = df; }

  void SetLightPos(const AmjuGL::Vec3& lp) { m_lightPos = lp; }

  virtual bool Init() = 0;
  virtual void Draw() = 0;

protected:
  DrawFunc m_drawFunc;
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

