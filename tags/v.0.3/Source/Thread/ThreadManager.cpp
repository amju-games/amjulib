/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThreadManager.cpp,v $
Revision 1.3  2008/05/08 10:58:07  jay
New memory management code

Revision 1.2  2006/10/27 22:23:44  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.1  2006/07/25 08:13:41  jay
Copied latest code from Uptime
Looks like this wasn't thread safe before!

Revision 1.2  2005/01/19 10:41:48  jay
Added GetThreadCount(), and added a mutex so add/delete/count functions
are MT-safe (!)

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
 
*/

#include "AmjuFirst.h"
#include "ThreadManager.h"
#include "AmjuFinal.h"

namespace Amju
{
void SingleThreadManager::AddThread(RCPtr<Thread> pThread)
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  m_threads[pThread->GetThreadId()] = pThread;
}

void SingleThreadManager::DeleteThread(int threadId)
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  m_threads.erase(threadId);
}

int SingleThreadManager::GetThreadCount() const
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  return m_threads.size();
}
}

