#include <AmjuFirst.h>
#include <Timer.h>
#include <Mutex.h>
#include "MessageQueue.h"
#include <AmjuFinal.h>

namespace Amju
{
float SecondsFromNow(float seconds)
{
  return TheMessageQueue::Instance()->GetTime() + seconds;
}

void MessageQueue::SetTime(float seconds)
{
  MutexLocker lock(m_mutex);
  m_time = seconds; 
}

float MessageQueue::GetTime() const
{
  return m_time;
}

void MessageQueue::Update()
{
  MutexLocker lock(m_mutex);

  // Update current time
  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;

  // Execute all messages whose timestamps have expired
  while (!m_q.empty())
  {
    PMessage pm =  *(m_q.begin());

    if (pm->m_time < m_time)
    {
      m_q.erase(m_q.begin());
      pm->Execute();
    }
    else
    {
      break;
    }
  }
}

void MessageQueue::Add(PMessage m)
{
  MutexLocker lock(m_mutex);
  m_q.insert(m);
}

}

