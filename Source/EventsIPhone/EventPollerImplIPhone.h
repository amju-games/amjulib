#ifndef EVENT_POLLER_IMPL_IPHONE_H
#define EVENT_POLLER_IMPL_IPHONE_H

#include <EventPoller.h>
#include <queue>

namespace Amju
{
class Event;

class EventPollerImplIPhone : public EventPollerImpl
{
public:
  virtual void Update(Listeners* pListeners);

  void QueueEvent(Event*); // event on heap, deleted in Update()

private:
  typedef std::queue<Event*> EventQueue;
  EventQueue m_q;
};
}

#endif


