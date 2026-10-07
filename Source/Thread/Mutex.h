// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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

