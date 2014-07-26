#pragma once

#include <RCPtr.h>
#include "Grid.h"

namespace Amju
{
class TerrainPatch : public RefCounted
{
public:
  bool Init(int x, int y, HeightFunc hf, int gridDensity, float gridSideLength);

  void Draw();

  // TODO
  // Set broad and detail diffuse maps
  
private:
  Grid m_grid;
};

typedef RCPtr<TerrainPatch> PTerrainPatch;
}

