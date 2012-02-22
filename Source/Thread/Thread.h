/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Thread.h,v $
Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.1  2006/07/25 08:13:02  jay
Copied latest code from Uptime

Revision 1.3  2005/01/19 10:40:50  jay
Added stop flag

Revision 1.2  2005/01/09 12:29:45  jay
Added static count for diagnosis

Revision 1.1  2005/01/03 11:08:54  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_THREAD_H_INCLUDED)
#define SCHMICKEN_THREAD_H_INCLUDED

#if defined(WIN32)
#else
#include <pthread.h>
#endif

#include "RCPtr.h"

namespace Amju
{
// Subclass Thread and override Work() to do something.
// NB All Threads must be SharedPtrs, created on the heap with new.
// The reason is that the Thread is stored as a SharedPtr by the ThreadManager.
// If you create a Thread on the stack IT WILL CRASH, because ThreadManager will
// try to delete it!
class Thread : public Shareable
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
  unsigned int m_threadId;

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

