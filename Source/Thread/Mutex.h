/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Mutex.h,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// Some threading code adapted from "Portable Thread Synchronization Using C++"
// http://world.std.com/~jimf/papers/c++sync/c++sync.html

#if !defined MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

#if defined(WIN32)
#include <windows.h>
#else
#ifdef GEKKO
#include "gekko_pthread.h"
#else
#include <pthread.h>
#endif
#endif
#include <Locker.h>

namespace Amju
{
class Mutex 
{
public:
  Mutex();
  ~Mutex();

  void Lock();
  void Unlock();

private:
  Mutex(const Mutex&);
  Mutex& operator=(const Mutex&);

private:
#if defined(WIN32)
  CRITICAL_SECTION m_crit;
#else
  pthread_mutex_t m_crit;
  volatile unsigned m_nestCount;
  volatile pthread_t m_owner;
#endif
};

typedef Locker<Mutex> MutexLocker;

}

#endif

