// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#ifndef EVENT_POLLER_IMPL_SDL
#define EVENT_POLLER_IMPL_SDL

#include "EventPoller.h"

namespace Amju
{
class EventPollerImplSDL : public EventPollerImpl
{
public:
  virtual void Update(Listeners* pListeners);
};
}

#endif
