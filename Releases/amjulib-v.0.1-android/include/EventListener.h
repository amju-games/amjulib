#ifndef AMJU_EVENT_LISTENER_H_INCLUDED
#define AMJU_EVENT_LISTENER_H_INCLUDED

#include <map>
#include <RCPtr.h>
#include "EventTypes.h"

namespace Amju
{
class EventListener : public RefCounted
{
public:
  virtual ~EventListener();
 
  // Handle an event.
  // Return true if the listener eats the event, in which case no more listeners are notified. 
  virtual bool OnKeyEvent(const KeyEvent&) { return false; }
  virtual bool OnCursorEvent(const CursorEvent&) { return false; }
  virtual bool OnRotationEvent(const RotationEvent&) { return false; }
  virtual bool OnJoyAxisEvent(const JoyAxisEvent&) { return false; }
  virtual bool OnButtonEvent(const ButtonEvent&) { return false; }
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) { return false; }
  virtual bool OnDoubleClickEvent(const DoubleClickEvent&) { return false; }
  virtual bool OnBalanceBoardEvent(const BalanceBoardEvent&) { return false; }
  virtual bool OnTextEvent(const TextEvent&) { return false; }
  virtual bool OnQuitEvent() { return false; }
  virtual bool OnResizeEvent(const ResizeEvent&) { return false; }
};

typedef RCPtr<EventListener> PEventListener;

// Map of priority to EventListener. Listeners are notified in ascending priority order.
// If a listener eats an event, no listeners with a lower priority are notified.
typedef std::multimap<int, PEventListener> Listeners;  
}

#endif // AMJU_EVENT_LISTENER_H_INCLUDED

