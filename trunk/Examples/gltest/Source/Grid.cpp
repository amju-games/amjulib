#include <Vec3.h>
#include "Grid.h"

namespace Amju
{
Grid::Grid()
{
}

void Grid::Build(int numDivs, float len, HeightFunc hf)
{
  AmjuGL::Tris tris;
  tris.reserve(numDivs * numDivs * 2);
  AmjuGL::Tri t;

  // Calc normals first
  std::vector<Vec3f> normals;
  if (hf)
  {
    int numNormals = (numDivs + 1) * (numDivs + 1);
std::cout << "Num Divs: " << numDivs << ", num normals: " << numNormals << "\n";
    normals.resize(numNormals);

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
        float y[4] = 
        {
          hf(x1, z0),
          hf(x0, z0),
          hf(x1, z1),
          hf(x0, z1)
        };
        Vec3f p[4] = 
        {
          Vec3f(x1, y[0], z0),
          Vec3f(x0, y[1], z0),
          Vec3f(x1, y[2], z1),
          Vec3f(x0, y[3], z1)
        };
        Vec3f n1 = CrossProduct(p[1] - p[0], p[2] - p[0]);
        n1.Normalise();
        Vec3f n2 = CrossProduct(p[1] - p[2], p[3] - p[2]);
        n2.Normalise();

std::cout << "n1: " << n1.x << ", " << n1.y << ", " << n1.z << "\n";
std::cout << "n2: " << n2.x << ", " << n2.y << ", " << n2.z << "\n";

        normals[(i + 1) * numDivs + j]     += n1;
        normals[ i      * numDivs + j]     += n1;
        normals[(i + 1) * numDivs + j + 1] += n1;

        normals[ i      * numDivs + j]     += n2;
        normals[(i + 1) * numDivs + j + 1] += n2;
        normals[ i      * numDivs + j + 1] += n2;
      }
    }
    Assert(normals.size() == numNormals);
    for (int i = 0; i < numNormals; i++)
    {
std::cout << "normal " << i << ": " << normals[i].x << ", " << normals[i].y << ", " << normals[i].z << "\n";
//      normals[i].Normalise();
    }
  }

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

      float y[4] = { 0 };
      if (hf)
      {
        y[0] = hf(x1, z0);
        y[1] = hf(x0, z0);
        y[2] = hf(x1, z1);
        y[3] = hf(x0, z1);
      }

      Vec3f n[4] = 
      {
        Vec3f(0, 1, 0),
        Vec3f(0, 1, 0),
        Vec3f(0, 1, 0),
        Vec3f(0, 1, 0)
      };

      if (hf)
      {
        n[0] = normals[(i + 1) * numDivs + j];
        n[1] = normals[ i      * numDivs + j];
        n[2] = normals[(i + 1) * numDivs + j + 1];
        n[3] = normals[ i      * numDivs + j + 1];
      }
      t.m_verts[0] = AmjuGL::Vert(x1, y[0], z0, u1, v0,  n[0].x, n[0].y, n[0].z);
      t.m_verts[1] = AmjuGL::Vert(x0, y[1], z0, u0, v0,  n[1].x, n[1].y, n[1].z);
      t.m_verts[2] = AmjuGL::Vert(x1, y[2], z1, u1, v1,  n[2].x, n[2].y, n[2].z);
      tris.push_back(t);

      t.m_verts[0] = AmjuGL::Vert(x1, y[2], z1, u1, v1,  n[2].x, n[2].y, n[2].z);
      t.m_verts[1] = AmjuGL::Vert(x0, y[1], z0, u0, v0,  n[1].x, n[1].y, n[1].z);
      t.m_verts[2] = AmjuGL::Vert(x0, y[3], z1, u0, v1,  n[3].x, n[3].y, n[3].z);
      tris.push_back(t);
    }
  }
  m_triList = MakeTriList(tris);
}

void Grid::Draw()
{
  m_triList->Draw();
}
}
