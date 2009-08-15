#ifndef EVENT_POLLER_IMPL_WIN32
#define EVENT_POLLER_IMPL_WIN32

#include "EventPoller.h"

namespace Amju
{
class EventPollerImplWin32 : public EventPollerImpl
{
public:
  virtual void Update(Listeners* pListeners);
};
}

#endif
