#include <AmjuFirst.h>
#include "EventListener.h"
#include "EventPoller.h"
#include <AmjuFinal.h>

namespace Amju
{
EventListener::~EventListener()
{
  // TODO The event poller may be dead now

  // Make sure this Listener is removed from the Poller's list!
  if (TheEventPoller::Instance()->HasListener(this))
  {
    // This should never be called, because if we are still in the EventPoller's
    //  list, our ref count will be 1, so we will never be in the dtor!!
    // BUT sometimes it happens -- this indicates a bad multiple inheritance
    //  diamond!!!
    Assert(0);
  }

}
}
