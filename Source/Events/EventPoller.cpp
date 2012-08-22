#include <iostream>
#include <AmjuAssert.h>
#include <Timer.h>
#include "EventPoller.h"

namespace Amju
{
EventPollerImpl* EventPoller::GetImpl()
{
  Assert(m_pImpl.GetPtr());
  return m_pImpl;
}

void EventPollerImpl::Update(Listeners* pListeners)
{
  while (!m_q.empty())
  {
    Event* event = m_q.front(); // Events on heap
    m_q.pop();

    NotifyListenersWithPriority(event, pListeners);

    delete event; // Events on heap
  }
}

void EventPollerImpl::QueueEvent(Event* event)
{
  m_q.push(event);
}

static KeyEvent lastKeyEvent;
static const float AUTO_REPEAT_START_DELAY = 0.5f; // TODO CONFIG 
static const float AUTO_REPEAT_RPT_DELAY = 0.1f;

static float rptTimer = 0;

enum AutoRepeatState { AMJU_AR_NONE, AMJU_AR_START_WAIT, AMJU_AR_RPT_WAIT }; 
static AutoRepeatState arState = AMJU_AR_NONE;

void HandleKey(KeyEvent* ke)
{
  if (ke->keyDown)
  {
    if (arState == AMJU_AR_NONE)
    {
      lastKeyEvent = *ke;
      arState = AMJU_AR_START_WAIT;
      rptTimer = AUTO_REPEAT_START_DELAY;
    }
  }
  else
  {
    arState = AMJU_AR_NONE;
    rptTimer = 0;
  }
}

void Repeat()
{
  if (arState == AMJU_AR_NONE)
  {
    return;
  }

  rptTimer -= TheTimer::Instance()->GetDt();

  if (rptTimer < 0)
  {
    // Have reached time limit, we now start repeating
    // TODO Queue Key Up event first ???
    TheEventPoller::Instance()->GetImpl()->QueueEvent(new KeyEvent(lastKeyEvent));

    arState = AMJU_AR_RPT_WAIT;
    rptTimer = AUTO_REPEAT_RPT_DELAY;
  }
}

void EventPollerImpl::NotifyListenersWithPriority(Event* event, Listeners* pListeners)
{
  if (dynamic_cast<KeyEvent*>(event)) // TODO Auto-repeatable flag ?
  {
    HandleKey((KeyEvent*)event);
  }

  int eaten = AMJU_MAX_PRIORITY + 1;
  for (Listeners::iterator it = pListeners->begin(); it != pListeners->end(); ++it)
  {
    if (it->first > eaten)
    {
      // This listener has a lower priority than a listener which ate the event
      break;
    }

    EventListener* listener = it->second;
    Assert(listener);

    if (event->UpdateListener(listener))
    {
      // Eaten
      eaten = it->first;
    }
  }
}

void EventPoller::Clear()
{
  m_listeners.clear();
}

void EventPoller::AddListener(EventListener* pListener, int priority)
{
  Assert(pListener);
  Assert(priority >= AMJU_MIN_PRIORITY);
  Assert(priority <= AMJU_MAX_PRIORITY);
  m_listeners.insert(std::make_pair(priority, pListener));
}

void EventPoller::RemoveListener(EventListener* pListener)
{
  for (Listeners::iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
  {
    if (it->second == pListener)
    {
      m_listeners.erase(it);
      return;
    }
  }
  Assert(0); // listener not found
}

bool EventPoller::HasListener(EventListener* pListener) const
{
  for (Listeners::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
  {
    if (it->second == pListener)
    {
      return true;
    }
  }
  return false;
}

void EventPoller::SetImpl(EventPollerImpl* pImpl)
{
  m_pImpl = pImpl;
}

void EventPoller::Update()
{
  Assert(m_pImpl.GetPtr());
  // Copy listeners so we can add or remove listeners in response to an event
  Listeners copyListeners = m_listeners;
  m_pImpl->Update(&copyListeners);
  Repeat();
}
}
