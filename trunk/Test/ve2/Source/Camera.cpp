#include <EventPoller.h>
#include <Game.h>
#include <DegRad.h>
#include <Timer.h>
#include "Camera.h"
#include "Portal.h"
#include "ROConfig.h"
#include "GameMode.h"

//#define CAM_DEBUG

namespace Amju
{
static float zDist = 0;
static float yRotAuto = 0; // rotation so we can see in doorways, etc.
static float yRotUser = 0; // user drag mouse - intended for edit mode, maybe also game mode
static float xRot = 0;

static bool leftDrag = false;
static bool rightDrag = false;
static bool midDrag = false;
static bool camKey = true;

static Vec3f posNoTarget;
static Vec3f posOffset;

Camera::Camera() : m_target(0)
{
  Reset();

  m_control = new CameraControl;
}

void Camera::Reset()
{
  // TODO Load settings, make configurable from server
  static const float CAM_Z = ROConfig()->GetFloat("cam-z");
  static const float XROT_START = DegToRad(ROConfig()->GetFloat("cam-xrot")); 
  
  zDist = CAM_Z;
  xRot = XROT_START;
  yRotAuto = 0;
  if (m_target)
  {
    // Face player (or whatever) face on
    yRotAuto = m_target->GetDir();
  }
  yRotUser = 0;
  leftDrag = false;
  rightDrag = false;
  midDrag = false;

  if (IsEditMode())
  {
    camKey = false;
  }
  else
  {
    camKey = true;
  }
}

void Camera::Update()
{
  //float dt = TheTimer::Instance()->GetDt();
  Vec3f pos;

  if (m_target)
  {
    pos = m_target->GetPos();

    // yRotAuto = 0;
#ifdef AUTO_ROTATE
    // Swing behind target
    float dir = m_target->GetDir() + 180.0f;
    float angleDiff = yRotAuto - dir;

    // Rotate to face m_dir, taking the shortest route (CW or CCW)
    if (fabs(angleDiff) < 0.1f) // TODO CONFIG
    {
      yRotAuto  = dir;
    }
    else
    {
      float ROT_SPEED = 1.0f; // TODO CONFIG
      if (yRotAuto > dir)
      {
        yRotAuto -= ROT_SPEED * dt * fabs(angleDiff);
      }
      else if (yRotAuto < dir)
      {
        yRotAuto += ROT_SPEED * dt * fabs(angleDiff);
      }
    }
#endif // AUTO_ROTATE

#ifdef PORTAL_ROTATE
    // TODO Get closest portal
    static PGameObject lastportal = 0;
    // We want to get the closest portal to the target, even if they have never intersected it.
    int pid = m_target->GetIgnorePortalId();
    if (pid > -1)
    {
      lastportal = TheGame::Instance()->GetGameObject(pid);
    }
    else
    {
      // Look for portals in this room, find closest.
      // TODO Optimise
      // just check periodically ?
      Portals portals = GetPortals();
      float bestSqDist = 999999.9f;
      for (Portals::iterator it = portals.begin(); it != portals.end(); ++it)
      {
        Portal* p = *it;
        float sqDist = (p->GetPos() - pos).SqLen();
        if (sqDist < bestSqDist)
        {
          bestSqDist = sqDist;
          lastportal = p;
        }
      } 
    }
    
    if (lastportal)
    {
      float pdist = (pos - lastportal->GetPos()).SqLen(); // sq dist from portal to player
      static const float MAX_DIST = ROConfig()->GetFloat("portal-max-dist", 200.0f); 
      static const float MAX_SQ_DIST = MAX_DIST * MAX_DIST; 
      if (pdist < MAX_SQ_DIST) 
      {
        yRotAuto = atan2(-pos.x, pos.z) * (1.0f - pdist / MAX_SQ_DIST); 

//std::cout << "SQ Dist from portal: " << pdist << " pos.z=" << pos.z << " pos.x=" << pos.x << " yRotAuto degs=" << RadToDeg(yRotAuto) << "\n";
      }
    }
#endif // PORTAL_ROTATE
  }
  else
  {
    pos = posNoTarget; 
  }
  pos += posOffset;

  float y = DegToRad(yRotAuto + yRotUser); 

  SetEyePos(Vec3f(
    pos.x + sin(y) * cos(xRot) * zDist, 
    pos.y + sin(xRot) * zDist, 
    pos.z + cos(y) * cos(xRot) * zDist));

  SetLookAtPos(pos);
}

CameraControl::CameraControl()
{
  TheEventPoller::Instance()->AddListener(this, -100); // High priority, right ?
}

bool CameraControl::OnKeyEvent(const KeyEvent& kb)
{
  // TODO Only alter camera if ALT or some other key is held down
  // Would like to use ALT like Maya but not getting the ALT/option key from GLUT...

  if (kb.keyType == AMJU_KEY_CHAR && kb.key == 'c') 
  {
    if (IsEditMode())
    { 
      camKey = kb.keyDown;
    }
    return false;
  }

  return false;
}

bool CameraControl::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;
  float dx = ce.x - oldx;
  float dy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  if (!camKey)
  {
    return false; 
  }

  bool b = false;
  if (leftDrag)
  {
    static const float XROT_SCALE = ROConfig()->GetFloat("cam-xrot-scale", 10.0f);
    static const float YROT_SCALE = ROConfig()->GetFloat("cam-yrot-scale", 10.0f);

    yRotUser -= RadToDeg(dx) * YROT_SCALE;
    xRot -= dy * XROT_SCALE;
    b = false; // true means we can't move off a button we have pressed down
  }
  if (rightDrag)
  {
    zDist += dy * 1000.0f; // TODO sensitivity
    b = true;
  }
  if (midDrag)
  {
    float y = DegToRad(yRotAuto + yRotUser); 

    posOffset.x -= dx * cos(y) * 1000.0f;
    posOffset.z += dx * sin(y) * 1000.0f;

    posOffset.y -= dy * 1000.0f;
    b = true;
  }

  return b;
}

bool CameraControl::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    leftDrag = mbe.isDown;
    if (camKey)
    {
#ifdef CAM_DEBUG
std::cout << "Camera handling mouse button event\n";
#endif
      return false;
    }
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    rightDrag = mbe.isDown;
    if (camKey)
    {
#ifdef CAM_DEBUG
std::cout << "Camera handling mouse button event\n";
#endif
      return true;
    }
  }
  else
  {
    midDrag = mbe.isDown;
    if (camKey)
    {
#ifdef CAM_DEBUG
std::cout << "Camera handling mouse button event\n";
#endif
      return true;
    }
  }

  return false;
}
}

