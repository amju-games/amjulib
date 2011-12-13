#include "EventListener.h"
#include "EventPoller.h"

namespace Amju
{
EventListener::~EventListener()
{
  // TODO The event poller may be dead now

  // Make sure this Listener is removed from the Poller's list!
  if (TheEventPoller::Instance()->HasListener(this))
  {
    TheEventPoller::Instance()->RemoveListener(this);
  }

}
}
