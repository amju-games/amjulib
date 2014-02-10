#include "Grid.h"

namespace Amju
{
Grid::Grid()
{
}

void Grid::Build(int numDivs, float len)
{
  m_tris.reserve(numDivs * numDivs * 2);
  AmjuGL::Tri t;

  float y = 0;

  for (int i = 0; i < numDivs; i++)
  {
    float u0 = (float)i / (float)numDivs;
    float u1 = (float)(i + 1) / (float)numDivs;

    for (int j = 0; j < numDivs; j++)
    {
      float v0 = (float)j / (float)numDivs;
      float v1 = (float)(j + 1) / (float)numDivs;

      float x0 = (u0 - 0.5f) * len;
      float x1 = (u1 - 0.5f) * len;
      float z0 = (v0 - 0.5f) * len;
      float z1 = (v1 - 0.5f) * len;

      t.m_verts[0] = AmjuGL::Vert(x1, y, z0, u1, v0,  0, 1, 0);
      t.m_verts[1] = AmjuGL::Vert(x0, y, z0, u0, v0,  0, 1, 0);
      t.m_verts[2] = AmjuGL::Vert(x1, y, z1, u1, v1,  0, 1, 0);
      m_tris.push_back(t);

      t.m_verts[0] = AmjuGL::Vert(x1, y, z1, u1, v1,  0, 1, 0);
      t.m_verts[1] = AmjuGL::Vert(x0, y, z0, u0, v0,  0, 1, 0);
      t.m_verts[2] = AmjuGL::Vert(x0, y, z1, u0, v1,  0, 1, 0);
      m_tris.push_back(t);
    }
  }
}

void Grid::Draw()
{
  AmjuGL::DrawTriList(m_tris);
}
}
