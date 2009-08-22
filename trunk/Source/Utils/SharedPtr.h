/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SharedPtr.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if ! defined(SCHMICKEN_SHARED_PTR_H_INCLUDED)
#define SCHMICKEN_SHARED_PTR_H_INCLUDED

#include "AmjuAssert.h"

namespace Amju 
{
// SharedPtr<T>
// ------------
// Originally written by Robin Schoolar.
//
// Replacement for boost::shared_ptr, using an 'intrusive' ref count.
// There's a big problem with boost::stared_ptr. It's very easy to end up
// with two objects that point to the same thing, with different ref counts.
// The result is a double-delete, and undefined behaviour.
// This SharedPtr only works with classes which are derived from Shareable,
// which is where the ref count lives. 

class Shareable
{
public:
  // Sadly, the ref count must be public for SharedPtr to access it (MSVC)
  mutable long m_shareable_refcount;

  long GetRefCount() const { return m_shareable_refcount; }

protected:
  Shareable() : m_shareable_refcount(0) {}
  Shareable(const Shareable&) : m_shareable_refcount(0) {}
  Shareable& operator= (const Shareable&) { return *this; }
  virtual ~Shareable() 
  { 
    Assert(m_shareable_refcount == 0); 
  }
};

template <class T>
class SharedPtr 
{
public:
  SharedPtr() : pRep(0) {}
  SharedPtr(T * pNew) : pRep(pNew) { Attach(); }
  SharedPtr(const SharedPtr & src) : pRep(src.pRep) { Attach(); }

  ~SharedPtr() { Detach(pRep); }

  SharedPtr& operator=(T* pNew) 
  {
      T * pOld = pRep;
      pRep = pNew;
      Attach();
      Detach(pOld);
      return *this ;
  }

  SharedPtr& operator=(const SharedPtr& src) 
  {
      T* pOld = pRep;
      pRep = src.pRep;
      Attach();
      Detach(pOld);
      return *this ;
  }

  // Use this to access the raw ptr - there's no conversion operator.
  T* GetPtr() const { return pRep; }
  // Now there is - more convenient
  operator T*() const { return pRep; }

  T* operator->() const { return pRep; }

private:
  // The pointer. Class T must be derived from Shareable.
  T* pRep;
  void Attach();
  static void Detach(T * p);
};

template <typename T>
void
SharedPtr<T>::Attach()
{
  if (pRep) 
  {
    ++(pRep->m_shareable_refcount);
  }
}

template <typename T>
void
SharedPtr<T>::Detach(T* p) 
{
  if (p) 
  {
    if (--(p->m_shareable_refcount) == 0)
    {
      delete p;
    } 
  }
}

} 

#endif 

