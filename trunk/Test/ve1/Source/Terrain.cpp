#include "Terrain.h"
#include <iostream>
#include <DrawOBB3.h>
#include <AmjuGL.h>
#include "Ve1SceneGraph.h"
#include <SceneMesh.h>
#include <GameObjectFactory.h>
#include <File.h>
#include <ReportError.h>
#include <CollisionMesh.h>
 
namespace Amju
{
static Terrain* currentTerrain = 0;

GameObject* CreateTerrain()
{
  return new Terrain;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Terrain::TYPENAME, CreateTerrain);

Terrain* Terrain::GetTerrain()
{
  // TODO
  return currentTerrain;
}

class TerrainSceneNode : public SceneMesh
{
public:
  TerrainSceneNode(Terrain* t) : m_terrain(t) {}
  CollisionMesh* GetCollisionMesh() { return & m_collMesh; }
  void CalcCollisionMesh(ObjMesh* mesh) { mesh->CalcCollisionMesh(&m_collMesh); }

private:
  Terrain* m_terrain;
  CollisionMesh m_collMesh;
};

Terrain::Terrain()
{
}

void Terrain::OnLocationEntry()
{
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(m_objFilename);
  if (!mesh)
  {
    ReportError("Terrain load: Failed to load terrain mesh " + m_objFilename);
    Assert(0);
    return; 
  }

  TerrainSceneNode* tsn = new TerrainSceneNode(this);
  tsn->SetMesh(mesh);
  tsn->UpdateBoundingVol(); // TODO just set a huge AABB
  tsn->CalcCollisionMesh(mesh);

  // Add Terrain to Scene Graph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(tsn);

  m_sceneNode = tsn;

//std::cout << "Terrain load: NICE! Created terrain object\n";

  currentTerrain = this; // for easy access to the current Terrain
}

CollisionMesh* Terrain::GetCollisionMesh()
{
  return m_sceneNode->GetCollisionMesh();
}

bool Terrain::Load(File* f)
{
  if (!f->GetDataLine(&m_objFilename))
  {
    f->ReportError("Expected .obj filename for terrain");
    return false;  
  }

//std::cout << "Terrain load: got obj mesh name: \"" << s << "\"\n";

  return true;
}

void Terrain::Draw()
{
  static OBB3 obb;
  obb.SetExtents(Vec3f(100, 1.0, 100));
  PushColour();
  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  DrawSolidOBB3(obb);
  PopColour();
}

void Terrain::Update()
{
}

const char* Terrain::TYPENAME = "terrain";

const char* Terrain::GetTypeName() const
{
  return TYPENAME;
}

Vec3f Terrain::GetMousePos(const LineSeg& seg)
{
  // Find intersection of line seg and terrain triangles - Octree would make this efficient
  // TODO
  // Assume ground plane at y=0. Find (x, z) at this y value.
  // Point p = P0 + t(P1 - P0)    =>   t = (p - P0) / (P1 - P0)
  //  p.y = 0, so t = - P0.y / (P1.y - P0.y)
  float m = (seg.p1.y - seg.p0.y); 
  if (m == 0)
  {
    std::cout << "Can't get mouse pos, line parallel to ground ?\n";
    return Vec3f();
  }
  else
  {
    float t = - seg.p0.y / m;
    if (t < 0 || t > 1)
    {
std::cout << "Unexpected value for t! " << t << "\n";
      return Vec3f();      
    }
std::cout << "t: " << t << "\n";
    return seg.GetPoint(t);
  }
}
}

