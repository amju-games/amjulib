#include "Camera.h"
#include <EventPoller.h>

namespace Amju
{
static const float CAM_Y = 100.0f;
static const float CAM_Z = 150.0f;
static float zDist = CAM_Z;
static float yRot = 0;

static bool leftDrag = false;
static bool rightDrag = false;
static bool midDrag = false;

static Vec3f posNoTarget;

Camera::Camera() : m_target(0)
{
  // Load settings

  TheEventPoller::Instance()->AddListener(this);
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

  if (leftDrag)
  {
    zDist -= dy * 100.0f;
//std::cout << "Camera: zDist: " << zDist << "\n";

//    return true;
  }
  if (rightDrag)
  {
    yRot += dx;
  }
  if (midDrag)
  {
    posNoTarget.x += dx * 100.0f;
    posNoTarget.y += dy * 100.0f;
  }

  return false;
}

bool Camera::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    leftDrag = mbe.isDown;
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    rightDrag = mbe.isDown;
  }
  else
  {
    midDrag = mbe.isDown;
  }

  return false;
}
}

