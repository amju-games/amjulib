#ifndef EVENT_POLLER_IMPL_WII_H_INCLUDED
#define EVENT_POLLER_IMPL_WII_H_INCLUDED

#include "EventPoller.h"

namespace Amju
{
class EventPollerImplWii : public EventPollerImpl
{
public:
  EventPollerImplWii();
  virtual void Update(Listeners* pListeners);
};
}

#endif // EVENT_POLLER_IMPL_WII_H_INCLUDED
