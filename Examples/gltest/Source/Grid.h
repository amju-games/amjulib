#ifndef GRID_H
#define GRID_H

#include <AmjuGL.h>
#include <TriList.h>
#include <Vec3.h>

namespace Amju
{
// Function which returns a y value for an (x, z) coordinate
typedef float (*HeightFunc)(float, float);

class Grid
{
public:
  Grid();
  void Build(int numDivisions, float len, HeightFunc hf = 0);
  void Rebuild(HeightFunc hf);
  void Draw();

protected:
  void RecalcNormals(HeightFunc hf);

protected:
  RCPtr<TriListDynamic> m_triList;
  AmjuGL::Tris m_tris;
  std::vector<Vec3f> m_normals;
  int m_numDivisions;
  float m_sideLength;
};
}

#endif
