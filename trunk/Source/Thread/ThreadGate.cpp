
#ifndef GEKKO

#include "AmjuFirst.h"
#include <iostream>
#ifndef WIN32
#include <errno.h>
#endif
#include "ThreadGate.h"
#include "AmjuFinal.h"

namespace Amju
{
ThreadGate::ThreadGate()
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  int r = pthread_cond_init(&m_cond, 0);
  r = pthread_mutex_init(&m_mutex, 0);
#endif
}

ThreadGate::~ThreadGate()
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  pthread_cond_destroy(&m_cond);
  pthread_mutex_destroy(&m_mutex);
#endif
}

void ThreadGate::Wait(unsigned int timeoutSecs)
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  // Get absolute timeout time, i.e. the time now plus the timeout.
  timespec abstime;
  time_t now;
  time(&now);
  abstime.tv_nsec = 0;
  abstime.tv_sec = now + timeoutSecs;

  pthread_mutex_lock(&m_mutex);

//std::cout << "THREADGATE waiting....\n";
  int r = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);

  pthread_mutex_unlock(&m_mutex);

  //int r = pthread_cond_wait(&m_cond, &m_mutex);
  if (r != 0)
  {
std::cout << "THREADGATE - error calling pthread_cond_timedwait\n";

    if (r == EINVAL)
    {
std::cout << "THREADGATE - bad values for args.\n";
    }
    else if (r == ETIMEDOUT)
    {
std::cout << "THREADGATE - failed because time already expired.\n"; 
    }
  }
//std::cout << "THREADGATE - opened or timed out.\n";
#endif
}

void ThreadGate::Wait()
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  pthread_mutex_lock(&m_mutex);
  pthread_cond_wait(&m_cond, &m_mutex);
  pthread_mutex_unlock(&m_mutex);
#endif
}

void ThreadGate::UnlockAndWait(MutexLocker& m)
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  // First get the lock on our mutex. This prevents Open() being called
  // until we unlock at the end of this function.
  pthread_mutex_lock(&m_mutex);

  // Now unlock the given mutex. 
  // Anything waiting on this mutex can now go ahead and call Open(), but
  // it will not happen before we Wait. 
  // This prevents the nasty possibility of Open being called before the Wait,
  // which could cause a deadlock. 
  m.Unlock();

  // pthread_cond_wait will unlock m_mutex, allowing a call to Open() to progress,
  // but not before we have startd to wait.
  pthread_cond_wait(&m_cond, &m_mutex);
  pthread_mutex_unlock(&m_mutex);
#endif
}

void ThreadGate::Open()
{
  AMJU_CALL_STACK;

#if defined(MACOSX) || defined(IPHONE)
  // Get a lock on the mutex before signalling the condition variable.
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
#endif
}
}

#endif // GEKKO

