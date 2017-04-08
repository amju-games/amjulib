#include <AmjuFirst.h>
#include "EventListener.h"
#include "EventPoller.h"
#include <AmjuFinal.h>

namespace Amju
{
EventListener::~EventListener()
{
  // TODO The event poller may be dead now
  TheEventPoller::Instance()->RemoveListener(this);
  Assert(!TheEventPoller::Instance()->HasListener(this));
}
}
