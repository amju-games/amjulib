#include <Timer.h>
#include <DegRad.h>
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
#include "Ve1ObjectChar.h"
#include "Ve1Character.h"
#include "Terrain.h"
#include "AvatarManager.h"
#include "Useful.h"

namespace Amju
{
static const float XSIZE = 10;
static const float YSIZE = 30; // TODO TEMP TEST, baddies can be any size

static const float SPEED = 50.0f; // TODO CONFIG

static const float GRAVITY = -50.0f; // TODO CONFIG

static const float BOUNCE_VEL = -50.0f;

static const char* TYPE_KEY = "type";
static const char* TEX_KEY = "tex";

Ve1ObjectChar::Ve1ObjectChar()
{
  m_dir = 0;
  m_dirCurrent = m_dir;
  m_isMoving = false;
  m_inNewLocation = false;
  m_collidingObject = 0;
  m_recalcHeading = false;

  SetKeyVal(TYPE_KEY, "0");
  SetKeyVal(TEX_KEY, "0");
}

void Ve1ObjectChar::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);
  if (key == TYPE_KEY)
  {
    int type = ToInt(val); // TODO overload Set to take an int
    Ve1Character* vc = dynamic_cast<Ve1Character*>(m_sceneNode.GetPtr());
    if (vc)
    {
      TheAvatarManager::Instance()->SetAvatar(type, vc);
    }
  }
  else if (key == TEX_KEY)
  {
    int texNum = ToInt(val);
    Ve1Character* vc = dynamic_cast<Ve1Character*>(m_sceneNode.GetPtr());
    if (vc)
    {
      TheAvatarManager::Instance()->SetTexture(texNum, vc);
    }
  }
  else if (key == "name")
  {
    SetName(val);
  }
  else if (key == "heading")
  {
    m_dir = ToFloat(val); 
  }
}

void Ve1ObjectChar::SetEditMenu(GuiMenu* menu)
{
//  menu->AddChild(new GuiMenuItem("Set data file...", new SetDataFile(GetId())));
}

void Ve1ObjectChar::SetIsColliding(GameObject* collidingObject)
{
  m_collidingObject = collidingObject;

  HasCollisionMesh* h = dynamic_cast<HasCollisionMesh*>(m_collidingObject);
  if (h)
  {
std::cout << "Shadow: adding collision mesh for " << *collidingObject << "\n";

    m_shadow->AddCollisionMesh(h->GetCollisionMesh());
  }  
}

void Ve1ObjectChar::HandleWalls(CollisionMesh* m, const Vec3f& oldPos, const Vec3f& newPos)
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

    // Skip tris facing away from us. This means the normal will be between 0 and 1.
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
    norm.Normalise();
    float dist = sqrt((oldPos - newPos).SqLen());
    m_pos += norm * dist;
  }
}

void Ve1ObjectChar::Update()
{
  // Not safe to do anything if the Terrain has not been created yet
  if (!TerrainReady())
  {
    return;
  }

  // TODO Not if underwater ?
  // TODO Put in base class, so we can drop things ?
  m_acc.y = GRAVITY;

  Ve1Object::Update();

  // Tell shadow the collision mesh it is casting onto
  // TODO Use octree etc
  // NB This only works if Terrain is activated before player!!
  // Also this crashes if Terrain not created at all yet!!
  // -> General problem of relying on another object already being in existence: you can't!!
  // When you start, and when you change locations, you must wait for the Terrain to be set up.
  // Otherwise we will be falling through floor, etc. if not crashing.

  // Just do once per location, but only when Terrain is valid.
  // I.e. have a flag, set it in OnLocationEntry. It's not complicated.

  if (m_inNewLocation)
  {
    m_shadow->ClearCollisionMeshes();
    m_shadow->AddCollisionMesh(GetTerrain()->GetCollisionMesh());
    m_inNewLocation = false;
  }

  // Get height for (x, z);
  float y = 0;
  // TODO Drop down
  // TODO Also check objects which have collision mesh

  // Get closest Y value to current, not the highest or lowest
  float groundY = m_pos.y;
  HasCollisionMesh* h = dynamic_cast<HasCollisionMesh*>(m_collidingObject);

  // Handle wall collisions with terrain and any building
  HandleWalls(GetTerrain()->GetCollisionMesh(), m_oldPos, m_pos);
  if (h)
  {
    HandleWalls(h->GetCollisionMesh(), m_oldPos, m_pos);
  }

  if (h && h->GetCollisionMesh()->GetClosestY(Vec2f(m_pos.x, m_pos.z), m_pos.y, &y))
  {
    groundY = y;
  }
  else if (GetTerrain()->GetCollisionMesh()->GetClosestY(Vec2f(m_pos.x, m_pos.z), m_pos.y, &y))
  {
    groundY = y;
  }

  if (m_pos.y < groundY)
  {
    m_pos.y = groundY;
    if (m_vel.y < BOUNCE_VEL) // less than i.e. more negative
    {
std::cout << "**BOUNCE**!?!?!?!\n";

      m_vel.y *= -0.6f;
      m_pos.y += m_vel.y * TheTimer::Instance()->GetDt(); // move up so not intersecting ground next frame
    }
    else
    {
      m_vel.y = 0; // ?
    }
  }

  // Recalc heading if we are not colliding
  if (m_collidingObject)
  {
    m_recalcHeading = true;
  }
  else if (m_recalcHeading)
  {
    m_recalcHeading = false;
    if (m_isMoving)
    {
      MoveTo(m_newPos);
    }
  }
  m_collidingObject = 0;

/*
  // Stop moving if we are close enough to the destination
  // TODO This ends up happening every frame, only do it if we are moving
  if (m_isMoving)
  {
    Vec3f dir = GetPos() - m_newPos;
    dir.y = 0; // ignore y coord for now
    if (dir.SqLen() < 1.0f) // TODO CONFIG
    {
      SetVel(Vec3f(0, 0, 0));
      m_newPos = GetPos();
      SetArrowVis(false);
      m_isMoving = false;
    }
  }
  else
  {
    Assert(GetVel().SqLen() == 0);
  }
*/

  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    //m_sceneNode->Update(); // done for whole scene graph elsewhere

    if (m_shadow)
    {
      // Set shadow AABB to same as Scene Node so we don't cull it by mistake
      *(m_shadow->GetAABB()) = *(m_sceneNode->GetAABB());
    }

    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);

    if (m_effect)
    {
      *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    }

    TurnToFaceDir();

    Ve1Character* vc = dynamic_cast<Ve1Character*>(m_sceneNode.GetPtr());
    if (vc && vc->GetMd2())
    {
      Vec3f v = m_vel;
      v.y = 0;
      float speed = v.SqLen();

      // TODO Simplify -- either moving or idle.
      // NB Speeds should be an avatar variable and level up

      static const float MAX_SPEED = 100.0f; // TODO CONFIG
      static const float RUN_SPEED = MAX_SPEED * 0.5f;
      static const float WALK_SPEED = RUN_SPEED * 0.5f;

      if (speed > RUN_SPEED)
      {
        vc->SetAnim("walk"); //"run");
      }
      else if (speed > WALK_SPEED)
      {
        vc->SetAnim("walk");
      }
      else
      {
        vc->SetAnim("stand");
      }
    }
  }
}

const std::string& Ve1ObjectChar::GetName() const
{
  return m_name;
}

void Ve1ObjectChar::SetName(const std::string& name)
{
  m_name = name;
}

bool Ve1ObjectChar::IsMoving() const
{
  return m_isMoving;
}

const Vec3f& Ve1ObjectChar::GetTargetPos()
{
  return m_newPos;
}

void Ve1ObjectChar::MoveTo(const Vec3f& newpos)
{
  m_newPos = newpos;
  m_isMoving = true;

  Vec3f dir = GetPos() - newpos;

  if (dir.SqLen() < 1.0f) // TODO CONFIG
  {
    SetVel(Vec3f(0, 0, 0));
  }
  else
  {
    dir.Normalise();
    SetVel(-dir * SPEED);

    // Work out direction to face
    SetDir(RadToDeg(atan2((double)m_vel.x, (double)m_vel.z)));
  }
}

void Ve1ObjectChar::SetDir(float degs)
{
  m_dir = degs;
}

void Ve1ObjectChar::TurnToFaceDir()
{
  Matrix mat;
  mat.RotateY(DegToRad(m_dirCurrent));
  mat.TranslateKeepRotation(m_pos);
  m_sceneNode->SetLocalTransform(mat);

  static const float ROT_SPEED = 10.0f; // TODO CONFIG
  float dt = TheTimer::Instance()->GetDt();
  float angleDiff = m_dir - m_dirCurrent;

  // Rotate to face m_dir, taking the shortest route (CW or CCW)
  if (fabs(angleDiff) < 10.0f)
  {
    m_dirCurrent = m_dir;
  }
  else
  {
    if (angleDiff < -180.0f)
    {
      m_dir += 360.0f;
    }
    else if (angleDiff > 180.0f)
    {
      m_dir -= 360.0f;
    }
    angleDiff = m_dir - m_dirCurrent;

    if (m_dirCurrent > m_dir)
    {
      m_dirCurrent -= ROT_SPEED * dt * fabs(angleDiff);
    }
    else if (m_dirCurrent < m_dir)
    {
      m_dirCurrent += ROT_SPEED * dt * fabs(angleDiff);
    }
  }
}

AABB* Ve1ObjectChar::GetAABB()
{
  return m_sceneNode->GetAABB();
}

void Ve1ObjectChar::ShowAttacked()
{
  m_effect->Start();
}

}

