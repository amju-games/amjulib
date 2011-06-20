#include "EventListener.h"
#include "EventPoller.h"

namespace Amju
{
EventListener::~EventListener()
{
  // Make sure this Listener is removed from the Poller's list!
  if (TheEventPoller::Instance()->HasListener(this))
  {
    TheEventPoller::Instance()->RemoveListener(this);
  }
}
}
