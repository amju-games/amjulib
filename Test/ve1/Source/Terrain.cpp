#include "Terrain.h"
#include <iostream>
#include <DrawOBB3.h>
#include <AmjuGL.h>
#include "Ve1SceneGraph.h"
#include <SceneMesh.h>
#include <GameObjectFactory.h>
#include <File.h>
 
namespace Amju
{
static Terrain* terrain = 0;

GameObject* CreateTerrain()
{
  return new Terrain;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Terrain::TYPENAME, CreateTerrain);

Terrain* Terrain::GetTerrain()
{
  // TODO
  return terrain;
}

class TerrainSceneNode : public SceneMesh
{
public:
  TerrainSceneNode(Terrain* t) : m_terrain(t) {}

private:
  Terrain* m_terrain;
};

Terrain::Terrain()
{
  terrain = this; // for easy access to the current Terrain
}

bool Terrain::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected .obj filename for terrain");
    return false;  
  }

//std::cout << "Terrain load: got obj mesh name: \"" << s << "\"\n";

  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(s);
  if (!mesh)
  {
    f->ReportError("Terrain load: Failed to load terrain mesh " + s);
    return false; 
  }

  TerrainSceneNode* tsn = new TerrainSceneNode(this);
  tsn->SetMesh(mesh);

  // Add Terrain to Scene Graph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(tsn);

//std::cout << "Terrain load: NICE! Created terrain object\n";

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

