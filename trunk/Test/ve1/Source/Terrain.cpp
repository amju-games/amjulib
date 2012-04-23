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
#include <StringUtils.h>
// Magic Software triangle-sphere intersect test
#include "Mgc/MgcIntr3DTriSphr.h"
// Mgc point-in-poly test
#include "Mgc/MgcCont2DPointInPolygon.h"
// Mgc distance from line to triangle
#include "Mgc/MgcDist3DLinTri.h"
// Mgc dist from point to triangle
#include "Mgc/MgcDist3DVecTri.h"
#include "Useful.h"
#include <Plane.h>

namespace Amju
{
static Terrain* currentTerrain = 0;

GameObject* CreateTerrain()
{
  return new Terrain;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Terrain::TYPENAME, CreateTerrain);

Terrain* GetTerrain()
{
  Assert(currentTerrain);
  return currentTerrain;
}

void ClearTerrain()
{
  currentTerrain = 0;
}

bool TerrainReady()
{
  return (currentTerrain != 0);
}

class TerrainSceneNode : public SceneMesh
{
public:
  TerrainSceneNode(Terrain* t) : m_terrain(t), m_collMesh(new CollisionMesh) {}
  CollisionMesh* GetCollisionMesh() { return m_collMesh; }
  void CalcCollisionMesh(ObjMesh* mesh) { mesh->CalcCollisionMesh(m_collMesh); }

private:
  Terrain* m_terrain;
  PCollisionMesh m_collMesh;
};

Terrain::Terrain()
{
}

bool Terrain::Load(File* f)
{
  if (!f->GetDataLine(&m_objFilename))
  {
    f->ReportError("Expected .obj filename for terrain");
    return false;  
  }

std::cout << "Terrain load: got obj mesh name: \"" << m_objFilename << "\"\n";

  return true;
}

void Terrain::OnLocationEntry()
{
  // Here we should already be loaded and ready to go.
  // Problem is other objects can have their OnLocationEntry() func called first.
  // It shouldn't matter what order this happens.

  currentTerrain = this;

  std::string path = GetFilePath(m_objFilename);
  std::string file = StripPath(m_objFilename);

//std::cout << "Terrain OnLocationEntry: Path: " << path << " Filename: " << file << "\n";

  std::string oldRoot = File::GetRoot();
  File::SetRoot(oldRoot + path, "/");

  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(file);

  File::SetRoot(oldRoot, "/");

  if (!mesh)
  {
    ReportError("Terrain load: Failed to load terrain mesh " + m_objFilename);
    Assert(0);
    return; 
  }

  TerrainSceneNode* tsn = new TerrainSceneNode(this);
  tsn->SetMesh(mesh);

  // TODO Not sure this actually does anything. What we want is to work out the AABB for the mesh.
  tsn->UpdateBoundingVol(); // TODO just set a huge AABB
  tsn->CalcCollisionMesh(mesh);

  // Add Terrain to Scene Graph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(tsn);

  m_sceneNode = tsn;

//std::cout << "Terrain load: NICE! Created terrain object\n";
}

CollisionMesh* Terrain::GetCollisionMesh()
{
  return m_sceneNode->GetCollisionMesh();
}


/*
void Terrain::Draw()
{
  static OBB3 obb;
  obb.SetExtents(Vec3f(100, 1.0, 100));
  PushColour();
  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  DrawSolidOBB3(obb);
  PopColour();
}
*/

void Terrain::Update()
{
}

const char* Terrain::TYPENAME = "terrain";

const char* Terrain::GetTypeName() const
{
  return TYPENAME;
}

bool Terrain::GetMousePos(const LineSeg& seg, Vec3f* pos)
{
  // Find all intersecting floor tris
  CollisionMesh::Tris tris;
  Capsule cap(seg, 1.0f); // TODO Radius ?

  if (!GetCollisionMesh()->Intersects(cap, &tris))
  {
std::cout << "Terrain: Line seg didn't intersect any tris\n";
    return false;
  }

  if (tris.empty())
  {
    // So Intersects() above should have returned false!?
    Assert(0);
    return false;
  }

  Mgc::Vector3 pt1(seg.p0.x, seg.p0.y, seg.p0.z);
  Mgc::Vector3 pt2(seg.p1.x - seg.p0.x, seg.p1.y - seg.p0.y, seg.p1.z - seg.p0.z);
  Mgc::Segment3 s;
  s.Origin() = pt1;
  s.Direction() = pt2;

  // Iterate over tris - find intersection points. Keep track of the closest point to p0.
  Vec3f closest = seg.p1;
  float closestSqDist = (seg.p1 - seg.p0).SqLen();

  int size = tris.size();

#ifdef TERRAIN_DEBUG
std::cout << "Found " << size << " tris....\n";
#endif

  bool foundOne = false;
  for (int i = 0; i < size; i++)
  {
    const Tri& t = tris[i];
      
    const Vec3f& a = t.m_verts[0];
    const Vec3f& b = t.m_verts[1];
    const Vec3f& c = t.m_verts[2];

    // Skip vertical tris
    Plane plane(a, b, c);
    if (plane.B() < 0.5f)
    {
      continue;
    }

#ifdef TERRAIN_DEBUG
std::cout << "This tri: " << a << " ; " << b << " ; " << c << "\n";
std::cout << "Seg: " << seg.p0 << " - " << seg.p1 << "\n";
#endif

    Mgc::Triangle3 tri;
    tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
    tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
    tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

    float q = 0; // parameter along line seg 0..1

#ifdef TERRAIN_DEBUG
    float d = 
#endif
    Mgc::SqrDistance(s, tri, &q);
    Vec3f p = seg.p0 + q * (seg.p1 - seg.p0);

#ifdef TERRAIN_DEBUG
std::cout << "SqrDistance intersect: " << p << "\n";

    // Dist d should be zero - the line seg intersects the tri.
std::cout << "d=" << d << "... expecting zero.\n";
#endif
 
    // Get dist from p0 to p so we get closest tri along line seg.
    float squareDist = (seg.p0 - p).SqLen(); // TODO or should that be p1 ?

    if (squareDist < closestSqDist)
    {
      closestSqDist = squareDist;
      closest = p;
      foundOne = true;
    }
  }

  *pos = closest;
  return foundOne;

/*
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
*/
}
}

