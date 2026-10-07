// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif

#if defined(MACOSX) || defined(IPHONE) || defined(ANDROID_NDK)
#include <sys/errno.h>
#endif

#include "Mutex.h"
#include "AmjuAssert.h"
#include <AmjuFinal.h>

namespace Amju
{
Mutex::Mutex() 
{
  AMJU_CALL_STACK;

#if defined(WIN32)
  InitializeCriticalSection(&m_crit);
#else
  m_nestCount = 0;
  pthread_mutex_init(&m_crit, 0);
  // m_owner does not need to be initialised
#endif
}

Mutex::~Mutex()
{
  AMJU_CALL_STACK;

#if defined(WIN32)
#else
  pthread_mutex_destroy(&m_crit);
#endif
}

void Mutex::Lock() 
{
  AMJU_CALL_STACK;

#if defined(WIN32)
  EnterCriticalSection(&m_crit);

#elif defined (GEKKO)
  pthread_mutex_lock(&m_crit);               

#else
  switch (pthread_mutex_trylock(&m_crit)) 
  {
  case 0:                                        
    break;

  case EBUSY:                                    
    if (m_nestCount > 0) 
    {                     
      if (m_owner == pthread_self()) 
      {       
        ++m_nestCount;
        return;
      } 
    } 
    pthread_mutex_lock(&m_crit);               
    break;
  }
  m_owner = pthread_self();                      
  m_nestCount = 1;

#endif 
}

void Mutex::Unlock() 
{
  AMJU_CALL_STACK;

#if defined(WIN32)
  LeaveCriticalSection(&m_crit);
#elif defined (GEKKO)
  pthread_mutex_unlock(&m_crit);
#else
  Assert(m_owner == pthread_self());
  Assert(m_nestCount > 0);
  if (--m_nestCount == 0) 
  {                                         
    pthread_mutex_unlock(&m_crit);
  } 
#endif
}
}
