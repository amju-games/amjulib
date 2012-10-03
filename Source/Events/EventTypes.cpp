#include "EventTypes.h"
#include "EventListener.h"

namespace Amju
{
bool QuitEvent::UpdateListener(EventListener* el)
{
  return el->OnQuitEvent();
}

bool CursorEvent::UpdateListener(EventListener* el)
{
  static const int MAX_CURSORS = 4; // ..?

  static float oldx[MAX_CURSORS];
  static float oldy[MAX_CURSORS];
  static bool first[MAX_CURSORS] = { true, true, true, true };

  if (first[controller])
  {
    first[controller] = false;
    oldx[controller] = x;
    oldy[controller] = y;
  }

  dx = x - oldx[controller];
  oldx[controller] = x;

  dy = y - oldy[controller];
  oldy[controller] = y;

//std::cout << "x: " << x << " y: " << y << " dx: " << dx << " dy: " << dy << "\n";

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
