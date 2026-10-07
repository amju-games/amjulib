// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

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
