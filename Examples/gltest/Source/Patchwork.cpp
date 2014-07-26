#include "Patchwork.h"

namespace Amju
{
float Flat(float x, float z)
{
  return 0;
}

void Patchwork::Init()
{
  TerrainPatch* tp = new TerrainPatch;
  tp->Init(0, 0, Flat, 3, 10.0f);
}

void Patchwork::Draw()
{
  for (auto it = m_patchmap.begin(); it != m_patchmap.end(); ++it)
  {
    it->second->Draw();
  }
}

}

