#include "Camera.h"
#include <EventPoller.h>

//#define CAM_DEBUG

namespace Amju
{
static const float CAM_Y = 100.0f;
static const float CAM_Z = 150.0f;
static float zDist = CAM_Z;
static float yRot = 0;

static bool leftDrag = false;
static bool rightDrag = false;
static bool midDrag = false;
static bool camKey = false;

static Vec3f posNoTarget;

Camera::Camera() : m_target(0)
{
  // Load settings

  TheEventPoller::Instance()->AddListener(this, -100); // High priority, right ?
}

void Camera::Update()
{
  if (m_target)
  {
    const Vec3f& pos = m_target->GetPos();
    SetEyePos(Vec3f(pos.x + sin(yRot) * zDist, pos.y + CAM_Y, pos.z + cos(yRot) * zDist));
    SetLookAtPos(pos);
  }
  else
  {
    const Vec3f& pos = posNoTarget; 
    SetEyePos(Vec3f(pos.x + sin(yRot) * zDist, pos.y + CAM_Y, pos.z + cos(yRot) * zDist));
    SetLookAtPos(pos);

    //SetEyePos(Vec3f(0, CAM_Y, CAM_Z));
    //SetLookAtPos(Vec3f(0, 0, 0));
  }
}

bool Camera::OnKeyEvent(const KeyEvent& kb)
{
  // TODO Only alter camera if ALT or some other key is held down
  if (kb.keyType == AMJU_KEY_CHAR && kb.key == 'c') // TODO S/b command
  {
    camKey = kb.keyDown;
  }

  return false;
}

bool Camera::OnCursorEvent(const CursorEvent& ce)
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
    yRot += dx;
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

bool Camera::OnMouseButtonEvent(const MouseButtonEvent& mbe)
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

