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

protected:
  // Notify all listeners of the event until the event is handled
  //  and only lower priority listeners have not been notified.
  void NotifyListenersWithPriority(Event* evemt, Listeners* pListeners);
};

typedef RCPtr<EventPollerImpl> PEventPollerImpl;

enum { AMJU_MIN_PRIORITY = -999, AMJU_MAX_PRIORITY = 999 };

class EventPoller : public NonCopyable
{
public:
  // Listeners have a priority. They are notified in ascending order.
  // If a listener eats the event, listeners with a lower priority are not notified.
  void AddListener(EventListener*, int priority = 0);
  void RemoveListener(EventListener*);
  bool HasListener(EventListener*) const;
  void SetListenerPriority(EventListener*, int priority);

  void SetImpl(EventPollerImpl*);
  EventPollerImpl* GetImpl();

  void Update();
  
private:
  PEventPollerImpl m_pImpl;
  Listeners m_listeners;
};

typedef SingletonNoDestroy<EventPoller> TheEventPoller;
}

#endif // AMJU_EVENT_POLLER_H_INCLUDED

