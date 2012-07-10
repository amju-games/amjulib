#include <Timer.h>
#include <DegRad.h>
#include "Ve1ObjectChar.h"
#include "Ve1Character.h"
#include "Terrain.h"

namespace Amju
{
static const float XSIZE = 10;
static const float YSIZE = 30; // TODO TEMP TEST, baddies can be any size

static const float SPEED = 50.0f; // TODO CONFIG

Ve1ObjectChar::Ve1ObjectChar()
{
  m_dir = 0;
  m_dirCurrent = m_dir;
  m_isMoving = false;
  m_inNewLocation = false;
  m_collidingObject = 0;
  m_recalcHeading = false;
}

void Ve1ObjectChar::SetIsColliding(GameObject* collidingObject)
{
  m_collidingObject = collidingObject;
}

void Ve1ObjectChar::Update()
{
  // Not safe to do anything if the Terrain has not been created yet
  if (!TerrainReady())
  {
    return;
  }

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
  if (GetTerrain()->GetCollisionMesh()->GetClosestY(Vec2f(m_pos.x, m_pos.z), m_pos.y, &y))
  {
    m_pos.y = y;
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

    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    *(m_shadow->GetAABB()) = *(m_sceneNode->GetAABB());

    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);

    TurnToFaceDir();

    if (m_sceneNode->GetMd2())
    {
      float speed = m_vel.SqLen();

      // TODO Simplify -- either moving or idle.
      // NB Speeds should be an avatar variable and level up

      static const float MAX_SPEED = 100.0f; // TODO CONFIG
      static const float RUN_SPEED = MAX_SPEED * 0.5f;
      static const float WALK_SPEED = RUN_SPEED * 0.5f;

      if (speed > RUN_SPEED)
      {
        m_sceneNode->SetAnim("walk"); //"run");
      }
      else if (speed > WALK_SPEED)
      {
        m_sceneNode->SetAnim("walk");
      }
      else
      {
        m_sceneNode->SetAnim("stand");
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
}

