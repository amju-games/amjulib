// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(SCHMICKEN_LOCKER_H_INCLUDED)
#define SCHMICKEN_LOCKER_H_INCLUDED

namespace Amju
{
// A Locker locks anything with Lock() and Unlock() functions.
// The idea is to lock the lockable thing by creating a Locker on the stack.
// When the Locker goes out of scope the lock is released.
// I.e. it's exception safe.

template<class T>
class Locker
{
public:
  Locker(T& lockable, bool doLock = true) : m_lockable(lockable)
  {
    m_isLocked = false;
    if (doLock)
    {
      Lock();
    }
  }

  ~Locker()
  {
    Unlock();
  }

  void Lock() 
  {
    if (m_isLocked)
    {
      return;
    }
    m_isLocked = true;
    m_lockable.Lock();
  }

  void Unlock()
  {
    if (!m_isLocked)
    {
      return;
    }
    m_isLocked = false;
    m_lockable.Unlock();
  }

private:
  T& m_lockable;
  bool m_isLocked;
};
}

#endif

