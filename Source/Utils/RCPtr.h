/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#ifndef AMJU_RC_PTR_INCLUDED
#define AMJU_RC_PTR_INCLUDED

#include "SharedPtr.h"

#define RCPtr SharedPtr
#define RefCounted Shareable


/*
namespace Amju 
{
class RefCounted
{
public:
  // Sadly, the ref count must be public for SharedPtr to access it (MSVC)
  mutable long m_shareable_refcount;

  long GetRefCount() const { return m_shareable_refcount; }

protected:
  RefCounted() : m_shareable_refcount(0) {}
  RefCounted(const RefCounted&) : m_shareable_refcount(0) {}
  RefCountede& operator= (const RefCounted&) { return *this; }
  virtual ~RefCounted() 
  { 
    Assert(m_shareable_refcount == 0); 
  }
};

template <class T>
class RCPtr 
{
public:
  RCPtr() : pRep(0) {}
  RCPtr(T * pNew) : pRep(pNew) { Attach(); }
  RCPtr(const RCPtr & src) : pRep(src.pRep) { Attach(); }

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
  T* GetPtr() const { return pRep ; }

  T* operator->() const { return pRep ; }

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
*/

#endif 

