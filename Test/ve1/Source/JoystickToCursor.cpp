#include <EventPoller.h>
#include <Timer.h>
#include "JoystickToCursor.h"

namespace Amju
{
JoystickToCursor* TheJoystickToCursor()
{
  static JoystickToCursor* js = 0;
  if (!js)
  {
    js = new JoystickToCursor;
  }
  return js;
}

JoystickToCursor::JoystickToCursor()
{
  m_hasEvent = false;
}

void JoystickToCursor::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  if (!m_hasEvent)
  {
    return;
  }

  // We should use whatever the cursor coord is currently (could have been moved by mouse)
  // m_coord = Mouse::Coord(); // TODO

  m_coord += m_joy * dt;
  if (m_coord.x < -1)
  {
    m_coord.x = -1;
  }
  if (m_coord.x > 1)
  {
    m_coord.x = 1;
  }
  if (m_coord.y < -1)
  {
    m_coord.y = -1;
  }
  if (m_coord.y > 1)
  {
    m_coord.y = 1;
  }

  CursorEvent* ce = new CursorEvent;
  ce->controller = 0; // TODO je.controller;
  ce->x = m_coord.x;
  ce->y = m_coord.y;

  TheEventPoller::Instance()->GetImpl()->QueueEvent(ce);

  if (m_joy.SqLen() < 0.01f) // TODO Sensitivity
  {
    m_hasEvent = false;
  }
}

bool JoystickToCursor::OnJoyAxisEvent(const JoyAxisEvent& je)
{
  // Make cursor event with the joystick data 
  // TODO sensitivity

  // TODO keep moving for as long as j/s is non-zero. This needs an Update(), as 
  // events are only generated when the j/s changes state.

  static const float SENSITIVITY = 1.5f; // TODO configurable per user

  // TODO
  // Check if m_coord is same as je within some tolerance, ignore if so.
  // (configurable tolerance too)

  m_joy.x = je.x * SENSITIVITY; 
  m_joy.y = je.y * SENSITIVITY;

  m_hasEvent = true;

  return true;
}

bool JoystickToCursor::OnButtonEvent(const ButtonEvent& be)
{
  MouseButtonEvent* mbe = 0;
  switch (be.button)
  {
  case AMJU_BUTTON_A:
    mbe = new MouseButtonEvent;
    mbe->button = AMJU_BUTTON_MOUSE_LEFT;
    break;

  default:
    return false;
  }

  mbe->isDown = be.isDown;

  TheEventPoller::Instance()->GetImpl()->QueueEvent(mbe);

  return true;
}
}

