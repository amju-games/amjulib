#include <EventPoller.h>
#include <Game.h>
#include <DegRad.h>
#include "Camera.h"

//#define CAM_DEBUG

namespace Amju
{
static const float CAM_Y = 100.0f;
static const float CAM_Z = 150.0f;
static float zDist = CAM_Z;
static float yRot = 0;
static float yRot2 = 0;

static bool leftDrag = false;
static bool rightDrag = false;
static bool midDrag = false;
static bool camKey = false;

static Vec3f posNoTarget;

Camera::Camera() : m_target(0)
{
  // Load settings

  m_control = new CameraControl;
}

void Camera::Update()
{
  if (m_target)
  {
    const Vec3f& pos = m_target->GetPos();
    yRot = 0;

    // TODO Get closest portal
    static PGameObject lastportal = 0;
    int pid = m_target->GetIgnorePortalId();
    if (pid > -1)
    {
      lastportal = TheGame::Instance()->GetGameObject(pid);
    }
    
    if (lastportal)
    {
      float pdist = (pos - lastportal->GetPos()).SqLen(); // sq dist from portal to player
      static const float MAX_DIST = 5000.0f; // TODO TEMP TEST
      if (pdist < MAX_DIST) 
      {
        yRot = atan2(-pos.x, pos.z) * (1.0f - pdist / MAX_DIST); 

//std::cout << "SQ Dist from portal: " << pdist << " pos.z=" << pos.z << " pos.x=" << pos.x << " yRot degs=" << RadToDeg(yRot) << "\n";
      }
    }
   
    float y = yRot + yRot2; 
    Vec3f eye(pos.x + sin(y) * zDist, pos.y + CAM_Y, pos.z + cos(y) * zDist);

    SetEyePos(eye); 
    SetLookAtPos(pos);
  }
  else
  {
    const Vec3f& pos = posNoTarget; 
    float y = yRot + yRot2; 
    SetEyePos(Vec3f(pos.x + sin(y) * zDist, pos.y + CAM_Y, pos.z + cos(y) * zDist));
    SetLookAtPos(pos);

    //SetEyePos(Vec3f(0, CAM_Y, CAM_Z));
    //SetLookAtPos(Vec3f(0, 0, 0));
  }
}

CameraControl::CameraControl()
{
  TheEventPoller::Instance()->AddListener(this, -100); // High priority, right ?
}

bool CameraControl::OnKeyEvent(const KeyEvent& kb)
{
  // TODO Only alter camera if ALT or some other key is held down
  if (kb.keyType == AMJU_KEY_CHAR && kb.key == 'c') // TODO S/b command
  {
    camKey = kb.keyDown;
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
    zDist -= dy * 100.0f;
    b = true;
  }
  if (rightDrag)
  {
    yRot2 += dx;
    b = true;
  }
  if (midDrag)
  {
    posNoTarget.x -= dx * 100.0f;
    posNoTarget.y -= dy * 100.0f;
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
      return true;
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

