#include <CollisionMesh.h>
#include "Squishy.h"

namespace Amju
{
Squishy::Squishy()
{
  m_volume = 0;

  m_mesh = nullptr; // TODO TEMP TEST
}

bool Squishy::Init(ObjMesh* mesh, float k)
{
  // TODO TEMP TEST
  m_mesh = mesh; // owned by resource manager

  

  // Create particles at verts. Connect with springs at triangle edges.

  // TODO Load in obj mesh, retaining index format - ObjMesh class converts to
  //  tri list.

  typedef std::map<Vec3f, int> Verts;
  Verts verts;

  typedef std::pair<int, int> Edge;
  typedef std::set<Edge> Edges;
  Edges edges;

  // Use collision mesh to get tri data - TEMP
  CollisionMesh cm;
  mesh->CalcCollisionMesh(&cm);
  const CollisionMesh::Tris& tris = cm.GetAllTris();
  for (const Tri& tri : tris)
  {
    int prevParticle = -1;
    for (int i = 0; i < 3; i++)
    {
      const Vec3f& v = tri.m_verts[i];

      int id = -1;
      // Look up v - if we've already found it, we have a particle already
      auto iter = verts.find(v);
      if (iter == verts.end())
      {
        // No particle created yet
        id = CreateParticle();
        Particle* p = GetParticle(id); // OK this is a bit crap
        p->SetPos(v);
        // Now what about (inv) mass of this particle?
        p->SetInvMass(1.0f); // TODO
      }
      else
      {
        id = iter->second;
      }
     
      // Create edge
      if (i > 0)
      {
        edges.insert(Edge(id, prevParticle));
      }
      prevParticle = id;
    }
  }

  // Create immovable particles at verts. Connect each vert to its immovable 
  //  particle with spring. Set k value to k given as parameter.

  return true; // TODO needed?
}

void Squishy::Draw()
{
  m_mesh->Draw();

  SpringSystem::Draw();
}

void Squishy::AddForce(const Vec3f& pos, const Vec3f& dir)
{
}

}
