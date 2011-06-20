#include "EventPollerImplGeneric.h"

namespace Amju
{
void EventPollerImplGeneric::Update(Listeners* pListeners)
{
  while (!m_q.empty())
  {
    Event* event = m_q.front(); // Events on heap
    m_q.pop();

    int eaten = AMJU_MAX_PRIORITY + 1;
    // When an event is eaten, listeners with equal priority are still notified.
    // Listeners with lower priority are not notified.
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
    delete event; // Events on heap
  }
}

void EventPollerImplGeneric::QueueEvent(Event* event)
{
  m_q.push(event);
}
}

