// Magic Software triangle-sphere intersect test
#include "Mgc/MgcIntr3DTriSphr.h"
// Mgc point-in-poly test
#include "Mgc/MgcCont2DPointInPolygon.h"
// Mgc distance from line to triangle
#include "Mgc/MgcDist3DLinTri.h"
// Mgc dist from point to triangle
#include "Mgc/MgcDist3DVecTri.h"
#include <CollisionMesh.h>
#include <Line3.h>
#include <Plane.h>
#include <Timer.h>
#include "Ve1Object.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "Ve1Node.h"
#include "GameMode.h"
#include "Ve1SceneGraph.h"
#include "Useful.h"
#include "HasCollisionMesh.h"
#include "Terrain.h"

namespace Amju
{
static const float BOUNCE_VEL = -50.0f;

Ve1Object::Ve1Object() : m_location(-1)
{
  m_isSelected = false;
  m_ignorePortalId = -1;
  m_isPickable = true;
  m_hidden = false;
  m_collidingObject = 0;
  m_shadow = new Shadow;
  m_inNewLocation = 2;
}

Ve1Object::~Ve1Object()
{
  OnLocationExit();
}

void Ve1Object::SetIsColliding(GameObject* collidingObject)
{
  m_collidingObject = collidingObject;

  HasCollisionMesh* h = dynamic_cast<HasCollisionMesh*>(m_collidingObject);
  if (h)
  {
std::cout << "Shadow: adding collision mesh for " << *collidingObject << "\n";

    m_shadow->AddCollisionMesh(h->GetCollisionMesh());
  }
}

void Ve1Object::HandleFloor(CollisionMesh* m)
{
  // Find ground pos: either the building etc we are colliding with..
  // Get closest Y value to current, not the highest or lowest
  float groundY = m_pos.y;
  float y = 0;
  if (m->GetClosestY(Vec2f(m_pos.x, m_pos.z), m_pos.y, &y))
  {
    groundY = y;
  }

  if (m_pos.y < groundY)
  {
    m_pos.y = groundY;
    if (m_vel.y < BOUNCE_VEL) // less than i.e. more negative
    {
std::cout << "**BOUNCE**!?!?!?!\n";

      m_vel.y *= -0.6f; // TODO CONFIG
      m_pos.y += m_vel.y * TheTimer::Instance()->GetDt(); // move up so not intersecting ground next frame
    }
    else
    {
      m_vel.y = 0; // ?
      OnBounceStop();
    }
  }
}

void Ve1Object::HandleWalls(CollisionMesh* m, const Vec3f& oldPos, const Vec3f& newPos)
{
  Vec3f dir = newPos - oldPos;
  LineSeg seg(oldPos, newPos);

  // Find all intersecting floor tris
  CollisionMesh::Tris tris;
  Capsule cap(seg, 1.0f); // TODO Radius

  if (!m->Intersects(cap, &tris))
  {
    return ;
  }

  if (tris.empty())
  {
    // So Intersects() above should have returned false!?
    Assert(0);
    return;
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
  Tri theTri;
  for (int i = 0; i < size; i++)
  {
    const Tri& t = tris[i];

    const Vec3f& a = t.m_verts[0];
    const Vec3f& b = t.m_verts[1];
    const Vec3f& c = t.m_verts[2];

    // Skip tris facing away from us. This means the dot prod of normal and dir will be between 0 and 1.
    Plane plane(a, b, c);
    if (DotProduct(plane.Normal(),  dir) > 0)
    {
      continue;
    }
    // Skip ground planes
    if (plane.B() > 0.1f)
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
      theTri = t; // remember triangle for later
    }
  }

  if (foundOne)
  {
    Vec3f norm = CrossProduct(theTri.m_verts[1] - theTri.m_verts[0], theTri.m_verts[2] - theTri.m_verts[0]);
    if (norm.SqLen() > 1.0f)
    {
      norm.Normalise();
      float dist = sqrt((oldPos - newPos).SqLen());
      m_pos += norm * dist;
    }
    else
    {
std::cout << "Dodgy triangle in collision mesh ?\n";      
    }
  }
}

bool Ve1Object::IsHidden() const
{
  return m_hidden;
}

void Ve1Object::SetHidden(bool b)
{
  m_hidden = b;
}

AABB* Ve1Object::GetAABB()
{
  return &m_aabb;
}

void Ve1Object::SetEditMenu(GuiMenu* menu)
{
}

void Ve1Object::SetSceneNode(SceneNode* n)
{
  Ve1Object::OnLocationExit();
  m_sceneNode = n;
}

void Ve1Object::CreateEditNode()
{
  if (GetGameMode() == AMJU_MODE_EDIT && !m_sceneNode)
  {
    static const float XSIZE = 20.0f; // TODO CONFIG
    static const float YSIZE = 40.0f;

    m_sceneNode = new Ve1Node(this);
    m_aabb.Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);
    *(m_sceneNode->GetAABB()) = m_aabb;

    SceneNode* name = new Ve1NameNode(this);
    AABB aabb(-10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f);
    *(name->GetAABB()) = aabb;
    m_sceneNode->AddChild(name);
  }
}

void Ve1Object::Update()
{
  m_oldPos = m_pos;

  if (m_sceneNode)
  {
    m_sceneNode->SetVisible(!m_hidden);
  }

  if (!IsHidden())
  {
    GameObject::Update();
  }

  if (m_inNewLocation > 0 && TerrainReady())
  {
    if (m_inNewLocation == 1)
    {
      m_shadow->ClearCollisionMeshes();
      m_shadow->AddCollisionMesh(GetTerrain()->GetCollisionMesh());
    }

    m_inNewLocation--;
  }
}

const Vec3f& Ve1Object::GetOldPos() const
{
  return m_oldPos;
}

bool Ve1Object::IsPickable() const 
{ 
  return m_isPickable; 
}

void Ve1Object::IgnorePortalId(int portalId)
{
  m_ignorePortalId = portalId;
}

int Ve1Object::GetIgnorePortalId() const
{
  return m_ignorePortalId;
}

bool Ve1Object::IsSelected() const
{
  return m_isSelected;
}

void Ve1Object::SetSelected(bool selected)
{
  m_isSelected = selected;
}

int Ve1Object::GetLocation() const
{
#ifdef _DEBUG
  if (m_location == -1)
  {
    std::cout << "Warning, object " << GetId() << " has location -1.\n"; 
  }
#endif

  //Assert(m_location != -1);

  return m_location;
}

void Ve1Object::SetLocation(int newLocation)
{
std::cout << "Setting location for " << m_id << " (" << GetTypeName() << "): " << newLocation << " (old: " << m_location << ")...\n";

  // Remove from game & scene graph etc if not in same location as local player
  // BUT object could also Enter the local player location!

  int oldLocation = m_location;

  if (oldLocation == newLocation)
  {
    return;
  }

  m_location = newLocation;

  int localPlayerLoc = GetLocalPlayerLocation();
  
  if (oldLocation == localPlayerLoc && newLocation != localPlayerLoc)
  {
    // We have just exited the local player location
    //OnLocationExit();  // ObjectManager will call, right ?
    // TODO notify ObjectManager ?
    TheObjectManager::Instance()->OnObjectChangeLocation(m_id); 
  }
  else if (oldLocation != localPlayerLoc && newLocation == localPlayerLoc)
  {
    // We have just entered the local player location
    //OnLocationEntry();
    TheObjectManager::Instance()->OnObjectChangeLocation(m_id); 
  }

  // NB What about when an object gets a Move or other change to state, and is not in the local
  //  player location...??!?!
  // It still gets updates if it exists in memory (i.e. ObjectManager has created it)
}

void Ve1Object::MoveTo(const Vec3f& pos)
{
  // Default behaviour: instantly change position
  SetPos(pos);
}

void Ve1Object::SetKeyVal(const std::string& key, const std::string& val)
{
  m_valmap[key] = val; 

  if (key == "hidden")
  {
std::cout << "Setting hidden flag for " << *this << " to: " << val << "\n";
    m_hidden = (val == "y");
  }
}

bool Ve1Object::Exists(const std::string& key) const
{
  return (m_valmap.find(key) != m_valmap.end());
}

const std::string& Ve1Object::GetVal(const std::string& key) const
{
  Assert(Exists(key));
  ValMap::const_iterator it = m_valmap.find(key);
  Assert(it != m_valmap.end());
  return it->second;
}

ValMap* Ve1Object::GetValMap()
{
  return &m_valmap;
}

void Ve1Object::OnLocationEntry()
{
  m_inNewLocation = 2;

  if (m_sceneNode)
  {
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->AddChild(m_sceneNode);

    m_sceneNode->AddChild(m_shadow.GetPtr());
  }
}

void Ve1Object::OnLocationExit()
{
  // Remove from SceneGraph
  if (m_sceneNode)
  {
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    if (root)
    {
      root->DelChild(m_sceneNode.GetPtr());
    }
  }
}

}

