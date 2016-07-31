#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <AmjuGL.h>
#include "DrawDecorator.h"
#include "Camera.h"
#include "Shader.h"
#include "RenderToTexture.h"

namespace Amju
{
// Render using a Shadow Map
class ShadowMap : public DrawDecorator
{
public:
  enum { DRAWABLE_TYPE_ID = 5 };

  // TODO get rid
  void SetLightPos(const AmjuGL::Vec3& lp) { m_lightPos = lp; }

  // Set the shader used in the second pass to read the depth texture and decide
  //  if each frag is in shadow.
  void SetShader(Shader* sh) { m_shader = sh; }

  void SetLight(const Camera& lightCam) { m_lightCam = lightCam; }
  void SetCamera(const Camera& cam) { m_cam = cam; }

protected:
  // TODO get rid
  AmjuGL::Vec3 m_lightPos;

  RCPtr<RenderToTexture> m_renderToTexture;
  RCPtr<Shader> m_shader;
  Camera m_lightCam;
  Camera m_cam;
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

