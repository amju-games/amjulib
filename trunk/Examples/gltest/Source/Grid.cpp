#include <Vec3.h>
#include "Grid.h"

namespace Amju
{
Grid::Grid() : m_numDivisions(0), m_sideLength(0)
{
}

void Grid::Build(int numDivs, float len, HeightFunc hf)
{
  m_numDivisions = numDivs;
  m_sideLength = len;
  m_tris.reserve(m_numDivisions * m_numDivisions * 2);
  m_triList = (TriListDynamic*)AmjuGL::Create(TriListDynamic::DRAWABLE_TYPE_ID);
  Rebuild(hf);
}

void Grid::RecalcNormals(HeightFunc hf)
{
  if (hf)
  {
    int numNormals = (m_numDivisions + 1) * (m_numDivisions + 1);
//std::cout << "Num Divs: " << m_numDivisions << ", num normals: " << numNormals << "\n";

    // Zero all normals
    m_normals.resize(numNormals);
    for (int i = 0; i < numNormals; i++)
    {
      m_normals[i] = Vec3f();
    }

    for (int i = 0; i < m_numDivisions; i++)
    {
      float u0 = (float)i / (float)m_numDivisions;
      float u1 = (float)(i + 1) / (float)m_numDivisions;

      for (int j = 0; j < m_numDivisions; j++)
      {
        float v0 = (float)j / (float)m_numDivisions;
        float v1 = (float)(j + 1) / (float)m_numDivisions;

        float x0 = (u0 - 0.5f) * m_sideLength;
        float x1 = (u1 - 0.5f) * m_sideLength;
        float z0 = (v0 - 0.5f) * m_sideLength;
        float z1 = (v1 - 0.5f) * m_sideLength;
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

//std::cout << "n1: " << n1.x << ", " << n1.y << ", " << n1.z << "\n";
//std::cout << "n2: " << n2.x << ", " << n2.y << ", " << n2.z << "\n";

        m_normals[(i + 1) * m_numDivisions + j]     += n1;
        m_normals[ i      * m_numDivisions + j]     += n1;
        m_normals[(i + 1) * m_numDivisions + j + 1] += n1;

        m_normals[ i      * m_numDivisions + j]     += n2;
        m_normals[(i + 1) * m_numDivisions + j + 1] += n2;
        m_normals[ i      * m_numDivisions + j + 1] += n2;
      }
    }
    Assert(m_normals.size() == numNormals);
    for (int i = 0; i < numNormals; i++)
    {
//std::cout << "normal " << i << ": " << m_normals[i].x << ", " << m_normals[i].y << ", " << m_normals[i].z << "\n";
//      m_normals[i].Normalise();
    }
  }
}

void Grid::Rebuild(HeightFunc hf)
{
  // Calc normals first
  RecalcNormals(hf); // TODO this is weird, why are we doing it like this

  AmjuGL::Tri t;
  m_tris.clear();

  for (int i = 0; i < m_numDivisions; i++)
  {
    float u0 = (float)i / (float)m_numDivisions;
    float u1 = (float)(i + 1) / (float)m_numDivisions;

    for (int j = 0; j < m_numDivisions; j++)
    {
      float v0 = (float)j / (float)m_numDivisions;
      float v1 = (float)(j + 1) / (float)m_numDivisions;

      float x0 = (u0 - 0.5f) * m_sideLength;
      float x1 = (u1 - 0.5f) * m_sideLength;
      float z0 = (v0 - 0.5f) * m_sideLength;
      float z1 = (v1 - 0.5f) * m_sideLength;

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
        n[0] = m_normals[(i + 1) * m_numDivisions + j];
        n[1] = m_normals[ i      * m_numDivisions + j];
        n[2] = m_normals[(i + 1) * m_numDivisions + j + 1];
        n[3] = m_normals[ i      * m_numDivisions + j + 1];
      }
      t.m_verts[0] = AmjuGL::Vert(x1, y[0], z0, u1, v0,  n[0].x, n[0].y, n[0].z);
      t.m_verts[1] = AmjuGL::Vert(x0, y[1], z0, u0, v0,  n[1].x, n[1].y, n[1].z);
      t.m_verts[2] = AmjuGL::Vert(x1, y[2], z1, u1, v1,  n[2].x, n[2].y, n[2].z);
      m_tris.push_back(t);


      t.m_verts[0] = AmjuGL::Vert(x1, y[2], z1, u1, v1,  n[2].x, n[2].y, n[2].z);
      t.m_verts[1] = AmjuGL::Vert(x0, y[1], z0, u0, v0,  n[1].x, n[1].y, n[1].z);
      t.m_verts[2] = AmjuGL::Vert(x0, y[3], z1, u0, v1,  n[3].x, n[3].y, n[3].z);
      m_tris.push_back(t);
    }
  }

  m_triList->Set(m_tris);
}

void Grid::Draw()
{
  m_triList->Draw();
}
}
