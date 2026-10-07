// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include <iostream>
#include "Thread.h"
#include "ThreadManager.h"

#if defined(WIN32)
#include <process.h>
#include <AmjuFinal.h>
#endif

//#define THREAD_DEBUG

namespace Amju
{
#if defined(WIN32)
void ThreadFunction(void *p)
#else
void* ThreadFunction(void* p)
#endif
{
  RCPtr<Thread> pThread = static_cast<Thread*>(p);
  pThread->Work();
  pThread->Finish();

  // This thread has finished. Tell the ThreadManager to delete it.
  ThreadManager::Instance()->DeleteThread(pThread->GetThreadId());
  
#if !defined(WIN32)
  return (void*)0;
#endif
}

Thread::Thread() : m_stop(false), m_threadId(0)
{
  AMJU_CALL_STACK;

  m_threadHandle = 0;
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

  m_threadHandle = static_cast<unsigned long>(_beginthread(&ThreadFunction, 0, this));
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
      m_threadId = (unsigned long int) thrHandle;
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
  return static_cast<unsigned int>(m_threadId);
}
}

