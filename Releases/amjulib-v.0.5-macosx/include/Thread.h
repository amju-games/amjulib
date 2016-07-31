#ifndef AMJU_THREAD_H_INCLUDED
#define AMJU_THREAD_H_INCLUDED

#if defined(WIN32)
#else
#ifdef GEKKO
#include "gekko_pthread.h"
#else
#include <pthread.h>
#endif
#endif

#include <RCPtr.h>

namespace Amju
{
// Subclass Thread and override Work() to do something.
// NB All Threads must be RCPtrs, created on the heap with new.
// The reason is that the Thread is stored as an RCPtr by the ThreadManager.
// If you create a Thread on the stack IT WILL CRASH, because ThreadManager will
// try to delete it!
class Thread : public RefCounted
{
public:
  Thread();
  virtual ~Thread();

  // Call this to start the thread. It will register itself with the
  // ThreadManager.
  void Start();

  // Set stop flag to true. The Work() function should test this flag and finish 
  // when it is set. 
  void Stop();

  unsigned int GetThreadId() const;

  // Conceptually protected, but these functions are called from a non-member function.

  // Override to do something.
  virtual void Work() = 0;

  // Called once Work() has returned. 
  virtual void Finish() {}

protected:
  // Set to true when the thread's Work() function should return.
  bool m_stop;

private:
  unsigned long int m_threadId;

#if defined(WIN32)
  unsigned long m_threadHandle;
#else
  pthread_t m_threadHandle;
#endif

  // Count no of threads created
  static int s_threadCount;
};
}

#endif

