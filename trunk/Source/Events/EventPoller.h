#ifndef AMJU_EVENT_POLLER_H_INCLUDED
#define AMJU_EVENT_POLLER_H_INCLUDED

#include "EventListener.h"
#include "RCPtr.h"
#include "Singleton.h"

namespace Amju
{
class EventPollerImpl : public RefCounted
{
public:
  virtual ~EventPollerImpl() {}
  virtual void Update(Listeners* pListeners) = 0;
};

typedef RCPtr<EventPollerImpl> PEventPollerImpl;

class EventPoller
{
public:
  void AddListener(EventListener*);
  void RemoveListener(EventListener*);
  bool HasListener(EventListener*) const;

  void SetImpl(EventPollerImpl*);
  EventPollerImpl* GetImpl();

  void Update();
  
private:
  PEventPollerImpl m_pImpl;
  Listeners m_listeners;
};

typedef Singleton<EventPoller> TheEventPoller;
}

#endif // AMJU_EVENT_POLLER_H_INCLUDED

