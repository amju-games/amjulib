#include <AmjuFirst.h>
#include <Timer.h>
#include <Mutex.h>
#include "MessageQueue.h"
#include <AmjuFinal.h>

namespace Amju
{
void MessageQueue::SetTime(float seconds)
{
  MutexLocker lock(m_mutex);
  m_time = seconds; 
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

	// TODO Comparing timestamp with process elapsed time as a float??
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
  MutexLocker lock(m_mutex);
  m_q.insert(m);
}

}

