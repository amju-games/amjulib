#pragma once

#include <Cubemap.h>
#include <ObjMesh.h>
#include <Singleton.h>
#include <Shader.h>
#include "GSBase.h"
#include "Grid.h"

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
  virtual void CreateTweakMenu() override;

private:
  Grid m_gridWater;
  Shader* m_shaderWater;
  RCPtr<ObjMesh> m_skybox;
  RCPtr<Texture> m_spheremap; // this may not be required with cube mapping
  RCPtr<Cubemap> m_cubemap;
  Shader* m_shaderTerrain;
  Texture* m_heightmapTerrain;
  Texture* m_diffuseTerrain;
  Texture* m_detailTerrain;
  Grid m_gridTerrain;
  int m_gridSizeWater;
  int m_gridSizeTerrain;
  bool m_drawWater;
  bool m_drawTerrain;
  bool m_isWireframeWater;
  bool m_isWireframeTerrain;
};
typedef Singleton<GSWater1> TheGSWater1;
} // namespace

