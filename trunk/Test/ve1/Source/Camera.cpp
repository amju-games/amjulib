#include <EventPoller.h>
#include <Game.h>
#include <DegRad.h>
#include "Camera.h"
#include "Portal.h"
#include "ROConfig.h"
#include "GameMode.h"

//#define CAM_DEBUG

namespace Amju
{
static float zDist = 0;
static float yRot = 0; // rotation so we can see in doorways, etc.
static float yRot2 = 0; // user drag mouse - intended for edit mode, maybe also game mode
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
  yRot = 0;
  yRot2 = 0;
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
  Vec3f pos;

  if (m_target)
  {
    pos = m_target->GetPos();
    yRot = 0;

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
        yRot = atan2(-pos.x, pos.z) * (1.0f - pdist / MAX_SQ_DIST); 

//std::cout << "SQ Dist from portal: " << pdist << " pos.z=" << pos.z << " pos.x=" << pos.x << " yRot degs=" << RadToDeg(yRot) << "\n";
      }
    }
  }
  else
  {
    pos = posNoTarget; 
  }
  pos += posOffset;

  float y = yRot + yRot2; 

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
    yRot2 -= dx;
    xRot -= dy;
    b = true;
  }
  if (rightDrag)
  {
    zDist += dy * 100.0f; // TODO sensitivity
    b = true;
  }
  if (midDrag)
  {
    float y = yRot + yRot2; 

    posOffset.x -= dx * cos(y) * 100.0f;
    posOffset.z += dx * sin(y) * 100.0f;

    posOffset.y -= dy * 100.0f;
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

