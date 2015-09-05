#pragma once

#include "SceneNodeProcMesh.h"
#include <Vec3.h>

namespace Amju
{
// Function which returns a y value for an (x, z) coordinate
typedef float (*HeightFunc)(float, float);

// Grid of tris in (x, z) plane. y coords can be varied by a function,
//  for terrain, water, etc.
class SceneNodeGrid : public SceneNodeProcMesh
{
public:
  SceneNodeGrid();
  void Build(int numDivisions, float len, HeightFunc hf = 0);
  void Rebuild(HeightFunc hf);
  void Draw() override;

protected:
  void RecalcNormals(HeightFunc hf);

protected:
  AmjuGL::Tris m_tris; // to rebuild tri list
  std::vector<Vec3f> m_normals;
  int m_numDivisions;
  float m_sideLength;
};
}

