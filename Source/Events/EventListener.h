#ifndef AMJU_EVENT_LISTENER_H_INCLUDED
#define AMJU_EVENT_LISTENER_H_INCLUDED

#include <set>
#include "EventTypes.h"

namespace Amju
{
class EventListener
{
public:
  virtual ~EventListener();
  
  virtual void OnKeyEvent(const KeyEvent&) {}
  virtual void OnCursorEvent(const CursorEvent&) {}
  virtual void OnRotationEvent(const RotationEvent&) {}
  virtual void OnJoyAxisEvent(const JoyAxisEvent&) {}
  virtual void OnButtonEvent(const ButtonEvent&) {}
  virtual void OnMouseButtonEvent(const MouseButtonEvent&) {}
  virtual void OnBalanceBoardEvent(const BalanceBoardEvent&) {}
  virtual void OnQuitEvent() {}
};

typedef std::set<EventListener*> Listeners;  
}

#endif // AMJU_EVENT_LISTENER_H_INCLUDED

