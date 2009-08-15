/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Mutex.cpp,v $
Revision 1.4  2008/05/18 16:04:28  jay
MSVC update

Revision 1.3  2008/05/08 10:58:05  jay
New memory management code

Revision 1.2  2006/01/20 12:48:24  jay
Gcc 4.0 fix: need to include sys/errno.h for EBUSY

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

// Some threading code adapted from "Portable Thread Synchronization Using C++"
// http://world.std.com/~jimf/papers/c++sync/c++sync.html

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#ifdef MACOSX
#include "sys/errno.h"
#endif

#include "Mutex.h"
#include "SchAssert.h"
#include "AmjuFinal.h"

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
