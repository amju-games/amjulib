#pragma once

#include <Cubemap.h>
#include <ObjMesh.h>
#include <Singleton.h>
#include <Shader.h>
#include "GSBase.h"

namespace Amju 
{
class GSWater1 : public GSBase
{
  GSWater1();
  friend class Singleton<GSWater1>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();
  virtual void SetUpTweakMenu() override;

private:
  Shader* m_shader;
  RCPtr<ObjMesh> m_skybox;
  RCPtr<Texture> m_spheremap; // this may not be required with cube mapping
  RCPtr<Cubemap> m_cubemap;
  bool m_isWireframe;
};
typedef Singleton<GSWater1> TheGSWater1;
} // namespace

