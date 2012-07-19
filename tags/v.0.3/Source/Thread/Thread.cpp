/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Thread.cpp,v $
Revision 1.3  2008/05/08 10:58:07  jay
New memory management code

Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.1  2006/07/25 08:13:02  jay
Copied latest code from Uptime

Revision 1.4  2005/02/28 20:34:23  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.3  2005/01/19 10:40:50  jay
Added stop flag

Revision 1.2  2005/01/09 12:29:45  jay
Added static count for diagnosis

Revision 1.1  2005/01/03 11:08:54  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "AmjuFirst.h"

#include <iostream>
#include "Thread.h"
#include "ThreadManager.h"

#if defined(WIN32)
#include <process.h>
#endif
#include "AmjuFinal.h"

//#define THREAD_DEBUG

namespace Amju
{
#if defined(WIN32)
void ThreadFunction(void *p)
#else
void* ThreadFunction(void* p)
#endif
{
  Thread* pThread = static_cast<Thread*>(p);
  pThread->Work();
  pThread->Finish();

  // This thread has finished. Tell the ThreadManager to delete it.
  ThreadManager::Instance()->DeleteThread(pThread->GetThreadId());
  
#if !defined(WIN32)
  return (void*)0;
#endif
}

Thread::Thread() : m_stop(false), m_threadId(0), m_threadHandle(0)
{
  AMJU_CALL_STACK;

}

Thread::~Thread()
{
  AMJU_CALL_STACK;

#ifdef THREAD_DEBUG
int c = ThreadManager::Instance()->GetThreadCount();
std::cout << "DESTROYING THREAD " << m_threadId << " COUNT IS NOW: "  << c << "\n";
#endif
}

void Thread::Start()
{
  AMJU_CALL_STACK;

#if defined(WIN32)

  m_threadHandle = _beginthread(&ThreadFunction, 0, this);
  m_threadId = m_threadHandle;

#elif defined (GEKKO)

  pthread_t thrHandle;
  if (!pthread_create(&thrHandle, 0, &ThreadFunction, this)) 
  {
      m_threadId = (int) thrHandle;
      m_threadHandle = thrHandle;
  }
  else
  {
std::cout << "FAILED TO CREATE THREAD\n";
    Assert(0);
    return;
  }

#else

  pthread_t thrHandle;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  if (!pthread_create(&thrHandle, &attr, &ThreadFunction, this)) 
  {
      m_threadId = (int) thrHandle;
      m_threadHandle = thrHandle;
  }
  else
  {
std::cout << "FAILED TO CREATE THREAD\n";
    Assert(0);
    return;
  }
#endif

  // Add this Thread to the ThreadManager.
  ThreadManager::Instance()->AddThread(this);

#ifdef THREAD_DEBUG
int c = ThreadManager::Instance()->GetThreadCount();
std::cout << "STARTED THREAD " << m_threadId << "  COUNT IS NOW " << c << "\n";
#endif

}

void Thread::Stop()
{
  AMJU_CALL_STACK;

  // The idea is that subclasses periodically test this variable in
  // their Work() function.
  // This doesn't need to be locked, because it's just set once
  // and then stays set until this object is destroyed.
  m_stop = true;
}

unsigned int Thread::GetThreadId() const
{
  return m_threadId;
}
}

