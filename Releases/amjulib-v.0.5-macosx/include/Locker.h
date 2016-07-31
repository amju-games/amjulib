/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Locker.h,v $
Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.1  2006/07/25 08:11:48  jay
Fix for worst, most stupid logic bug ever. This never worked before this fix.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
 
*/

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

