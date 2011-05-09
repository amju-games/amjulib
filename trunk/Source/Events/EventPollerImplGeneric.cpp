#include "EventPollerImplGeneric.h"

namespace Amju
{
void EventPollerImplGeneric::Update(Listeners* pListeners)
{
  while (!m_q.empty())
  {
    Event* event = m_q.front(); // Events on heap
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

void EventPollerImplGeneric::QueueEvent(Event* event)
{
  m_q.push(event);
}
}

