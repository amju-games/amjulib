/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#ifndef AMJU_RC_PTR_INCLUDED
#define AMJU_RC_PTR_INCLUDED

#include <algorithm>
#include "AmjuAssert.h"

namespace Amju 
{
// Thanks to Robin Schoolar

class RefCounted
{
private:
  template <class T> friend class RCPtr;
  mutable int m_shareable_refcount;

public:
  int GetRefCount() const { return m_shareable_refcount; }

protected:
  RefCounted() : m_shareable_refcount(0) {}
  RefCounted(const RefCounted&) : m_shareable_refcount(0) {}
  RefCounted& operator=(const RefCounted&) { return *this; }
  virtual ~RefCounted() 
  { 
    Assert(m_shareable_refcount == 0); 
  }
};

template <class T>
class RCPtr 
{
public:
  RCPtr() : m_ptr(0) {}
  RCPtr(T * pNew) : m_ptr(pNew) { Attach(); }
  RCPtr(const RCPtr & src) : m_ptr(src.m_ptr) { Attach(); }

  ~RCPtr() { Detach(m_ptr); }

  RCPtr& operator=(const RCPtr& src) 
  {
    RCPtr temp(src);
    std::swap(m_ptr, temp.m_ptr);
    return *this;
  }

  // Use this to access the raw ptr if necessary 
  T* GetPtr() const { return m_ptr; }

  // Type conversion operator
  operator T*() const { return m_ptr; }

  T* operator->() const { return m_ptr; }

private:
  T* m_ptr;
  void Attach()
  {
    if (m_ptr) 
    {
      ++(m_ptr->m_shareable_refcount);
    }
  }

  static void Detach(T * p)
  {
    if (p) 
    {
      if (--(p->m_shareable_refcount) == 0)
      {
        delete p;
      } 
    }
  }
};

} 

#endif 

