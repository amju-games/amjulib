#include "EventPollerImplIPhone.h"

namespace Amju
{
void EventPollerImplIPhone::Update(Listeners* pListeners)
{
  while (!m_q.empty())
  {
    Event* event = m_q.top(); // Events on heap
    m_q.pop();

    for (Listeners::iterator it = pListeners->begin(); it != pListeners->end(); ++it)
    {
      EventListener* listener = *it;
      Assert(listener);

      event->UpdateListener(listener);
    }
    delete event; // Events on heap
  }
}

  void EventPollerImplIPhone::QueueCursorEvent(const CursorEvent&);
  void EventPollerImplIPhone::QueueRotationEvent(const RotationEvent&);
  void EventPollerImplIPhone::OnMouseButtonEvent(const MouseButtonEvent&);
  void EventPollerImplIPhone::OnBalanceBoardEvent(const BalanceBoardEvent&);
  void EventPollerImplIPhone::OnQuitEvent();

}

