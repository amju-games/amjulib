#include <AmjuFirst.h>
#include "EventTypes.h"
#include "EventListener.h"
#include <AmjuFinal.h>

namespace Amju
{
bool QuitEvent::UpdateListener(EventListener* el)
{
  return el->OnQuitEvent();
}

bool CursorEvent::UpdateListener(EventListener* el)
{
  return el->OnCursorEvent(*this);
}

bool KeyEvent::UpdateListener(EventListener* el)
{
  return el->OnKeyEvent(*this);
}

bool JoyAxisEvent::UpdateListener(EventListener* el)
{
  return el->OnJoyAxisEvent(*this);
}

bool ButtonEvent::UpdateListener(EventListener* el)
{
  return el->OnButtonEvent(*this);
}

bool MouseButtonEvent::UpdateListener(EventListener* el)
{
  return el->OnMouseButtonEvent(*this);
}

bool DoubleClickEvent::UpdateListener(EventListener* el)
{
  return el->OnDoubleClickEvent(*this);
}

bool RotationEvent::UpdateListener(EventListener* el)
{
  return el->OnRotationEvent(*this);
}

static float bb_zero_x = 0;
static float bb_zero_y = 0;

void BalanceBoardEvent::SetZeroCoords(float x, float y)
{
  bb_zero_x = x;
  bb_zero_y = y;
}

BalanceBoardEvent::BalanceBoardEvent(float ux, float uy) // uncalibrated
{
  x = ux;
  y = uy;
}
  
// Get adjusted coord so if it's the same as the values set in SetZeroCoords,
//  you get (0, 0).
Vec2f BalanceBoardEvent::GetCalibratedCoord() const
{
  return Vec2f(x - bb_zero_x, y - bb_zero_y);
}

Vec2f BalanceBoardEvent::GetUncalibratedCoord() const
{
  return Vec2f(x, y);
}

bool BalanceBoardEvent::UpdateListener(EventListener* el)
{
  return el->OnBalanceBoardEvent(*this);
}

bool TextEvent::UpdateListener(EventListener* el)
{
  return el->OnTextEvent(*this);
}
  
bool ResizeEvent::UpdateListener(EventListener* el)
{
  return el->OnResizeEvent(*this);
}
}
