#include <AmjuFirst.h>
#include "MessageQueue.h"
#include <Timer.h>
#include <AmjuFinal.h>

namespace Amju
{
void MessageQueue::Update()
{
  // Update current time
//  float dt = TheTimer::Instance()->GetDt();
//  m_time += dt;

  // Execute all messages whose timestamps have expired
  while (!m_q.empty())
  {
    PMessage pm =  *(m_q.begin());

    if (pm->m_time < m_time)
    {
      pm->Execute();
      m_q.erase(m_q.begin());
    }
    else
    {
      break;
    }
  }
}

void MessageQueue::Add(PMessage m)
{
  m_q.insert(m);
}

}

