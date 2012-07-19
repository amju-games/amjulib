/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: ThreadGate.h,v $
Revision 1.2  2006/10/27 22:23:44  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.1  2006/07/25 08:17:20  jay
Copied ThreadGate over from Uptime - currently Mac only

Revision 1.2  2005/02/28 20:34:23  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.1  2005/01/19 10:22:59  jay
Added ThreadGate: allows a thread to block until the gate is opened
by another thread.

*/

#ifndef THREAD_GATE_H_INCLUDED
#define THREAD_GATE_H_INCLUDED

#ifndef GEKKO // no Wii support 

#if defined(MACOSX) || defined (IPHONE)
#include <pthread.h>
#endif
#include "Mutex.h"

namespace Amju 
{
// ThreadGate lets a thread block, using Wait(), until it is
// either unbocked by another thread, or optionally, the Wait
// state times out.
class ThreadGate 
{
public:
  ThreadGate();
  ~ThreadGate();

  // Wait until timeout expires or Open() is called.
  void Wait(unsigned int timeoutSecs);
 
  // Wait forever (until Opened)
  void Wait();

  // Like Wait, but gets a lock on m_mutex, then unlocks the given
  // MutexLocker, before waiting.
  // This lets you do some stuff, then wait, in the calling code,
  // without the possibility of Open() being called before Wait().
  void UnlockAndWait(MutexLocker&);

  // A second thread calls this to open the gate, allowing
  // the waiting thread to 'go through'.
  void Open();

protected:
#if defined(MACOSX) || defined(IPHONE)
  // Condition variable
  pthread_cond_t m_cond;
  // Mutex, which function pthread_cond_wait requires.
  pthread_mutex_t m_mutex;
#endif
};

}
#endif // GEKKO
#endif // include guard
