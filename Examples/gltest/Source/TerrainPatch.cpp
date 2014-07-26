#include "TerrainPatch.h"

namespace Amju
{
bool TerrainPatch::Init(int x, int y, 
  HeightFunc hf, int gridDensity, float gridSideLength)
{
  m_grid.Build(gridDensity, gridSideLength, hf);

  return true;
}

void TerrainPatch::Draw()
{
  // TODO multi texture shader
  // Set textures
  m_grid.Draw();
}

}


