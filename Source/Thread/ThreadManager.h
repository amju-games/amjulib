// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(SCHMICKEN_THREAD_MANAGER_H_INCLUDED)
#define SCHMICKEN_THREAD_MANAGER_H_INCLUDED

#include <map>
#include "Thread.h"
#include <RCPtr.h>
#include "Mutex.h"
#include <Singleton.h>

namespace Amju
{
// This object (Singleton) holds all Threads currently in existence.
// When a Thread finishes executing, the ThreadManager cleans it up.
class SingleThreadManager
{
public:
  // Called by the Thread ctor.
  // This ensures that a Thread object exists for the lifetime of 
  // the thread execution.
  void AddThread(RCPtr<Thread> pThread);

  // Called by a thread function after Work() and Finish() have completed.
  // This decrements the ref count so the Thread object can be destroyed.
  void DeleteThread(int threadId);

  int GetThreadCount() const;

private:
  // Map thread IDs to Threads.
  typedef std::map<int, RCPtr<Thread> > ThreadMap;
  ThreadMap m_threads;

private:
  SingleThreadManager() {}
  SingleThreadManager(const SingleThreadManager&);
  SingleThreadManager& operator=(const SingleThreadManager&);
  friend class Singleton<SingleThreadManager>;

  mutable Mutex m_mutex;
};

typedef Singleton<SingleThreadManager> ThreadManager;
}

#endif
