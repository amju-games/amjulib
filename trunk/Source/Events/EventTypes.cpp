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
