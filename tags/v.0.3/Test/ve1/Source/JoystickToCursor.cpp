#include <EventPoller.h>
#include "JoystickToCursor.h"

namespace Amju
{
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

  m_coord.x = je.x * SENSITIVITY; 
  m_coord.y = je.y * SENSITIVITY;

  CursorEvent* ce = new CursorEvent;
  ce->controller = je.controller;
  ce->x = m_coord.x;
  ce->y = m_coord.y;

  TheEventPoller::Instance()->GetImpl()->QueueEvent(ce);

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

