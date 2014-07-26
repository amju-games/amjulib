#pragma once

#include <map>
#include <Vec2.h>
#include "TerrainPatch.h"

namespace Amju
{
// TODO s/ton?

class Patchwork
{
public:
  void Init();
  void Draw();

private:
  // Container of TerrainPatches, centred at an (x, z) coord in the world.
  // Patches are created when the bounding box for the patch is visible.
  // Patches are destroyed when the distance exceeds some threshold, with 
  //  some hysteresis to prevent thrashing.

  // Far terrain patches are lower level of detail than near ones. 
  // So patches can be rebuilt at a different detail level.

  typedef std::map<Vec2i, PTerrainPatch> PatchMap;
  PatchMap m_patchmap;
};
}

