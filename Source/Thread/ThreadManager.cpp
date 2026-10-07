// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "ThreadManager.h"
#include <AmjuFinal.h>

namespace Amju
{
void SingleThreadManager::AddThread(RCPtr<Thread> pThread)
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  m_threads[pThread->GetThreadId()] = pThread;
}

void SingleThreadManager::DeleteThread(int threadId)
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  m_threads.erase(threadId);
}

int SingleThreadManager::GetThreadCount() const
{
  AMJU_CALL_STACK;

  MutexLocker lock(m_mutex);
  return static_cast<int>(m_threads.size());
}
}

