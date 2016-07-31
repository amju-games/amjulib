#pragma once

#include <Cubemap.h>
#include <ObjMesh.h>
#include <Shader.h>
#include "GSBase.h"
#include "Grid.h"

namespace Amju 
{
class GSWaterBase : public GSBase
{
protected:
  GSWaterBase();

public:
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void OnActive() override;
  virtual void CreateTweakMenu() override;

protected:
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

} // namespace

