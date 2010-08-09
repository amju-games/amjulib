#include "EventTypes.h"
#include "EventListener.h"

namespace Amju
{
void CursorEvent::UpdateListener(EventListener* el)
{
  el->OnCursorEvent(*this);
}

void KeyEvent::UpdateListener(EventListener* el)
{
  el->OnKeyEvent(*this);
}

void JoyAxisEvent::UpdateListener(EventListener* el)
{
  el->OnJoyAxisEvent(*this);
}

void ButtonEvent::UpdateListener(EventListener* el)
{
  el->OnButtonEvent(*this);
}

void MouseButtonEvent::UpdateListener(EventListener* el)
{
  el->OnMouseButtonEvent(*this);
}

void RotationEvent::UpdateListener(EventListener* el)
{
  el->OnRotationEvent(*this);
}

void BalanceBoardEvent::UpdateListener(EventListener* el)
{
  el->OnBalanceBoardEvent(*this);
}
}
