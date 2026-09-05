/*
Amju Games source code (c) Copyright Juliet Colman 2009
*/

#pragma once

#include "AmjuAssert.h"

namespace Amju 
{
// Thanks to Robin Schoolar

class RefCounted
{
private:
  template <class T> friend class RCPtr;
  template <class T> friend class WeakPtr;
  int m_shareable_refcount = 0;
  int m_weakcount = 0;

public:
  int GetRefCount() const { return m_shareable_refcount; }
  int GetWeakCount() const { return m_weakcount; }

protected:
  RefCounted() : m_shareable_refcount(0) {}
  RefCounted(const RefCounted&) : m_shareable_refcount(0) {}
  RefCounted& operator=(const RefCounted&) { return *this; }
  virtual ~RefCounted() 
  { 
    Assert(m_shareable_refcount == 0); 
    Assert(m_weakcount == 0);
  }
};

template <class T>
class RCPtr 
{
public:
  RCPtr() : m_ptr(0) {}
  RCPtr(T * pNew) : m_ptr(pNew) { Attach(); }
  RCPtr(const RCPtr & src) : m_ptr(src.m_ptr) { Attach(); }

  ~RCPtr() { Detach(); }

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
      Assert(m_ptr->m_shareable_refcount >= 0);
      ++(m_ptr->m_shareable_refcount);
    }
  }

  void Detach()
  {
    if (m_ptr) 
    {
      Assert(m_ptr->m_shareable_refcount > 0);
      if (--(m_ptr->m_shareable_refcount) == 0)
      {
        delete m_ptr;
      } 
    }
  }
};

template <class T>
class WeakPtr
{
public:
  WeakPtr() = default;
  WeakPtr(T* pNew) : m_ptr(pNew) { IncWeakCount(); }
  WeakPtr(const WeakPtr& src) : m_ptr(src.m_ptr) { IncWeakCount(); }

  ~WeakPtr() { DecWeakCount(); }

  WeakPtr& operator=(const WeakPtr& src)
  {
    WeakPtr temp(src);
    std::swap(m_ptr, temp.m_ptr);
    return *this;
  }

  // Use this to access the raw ptr if necessary 
  T* GetPtr() const { return m_ptr; }

  // Type conversion operator
  operator T* () const { return m_ptr; }

  T* operator->() const { return m_ptr; }

private:
  T* m_ptr = nullptr;
  void IncWeakCount()
  {
    if (m_ptr)
    {
      Assert(m_ptr->m_weakcount >= 0);
      ++(m_ptr->m_weakcount);
    }
  }

  void DecWeakCount()
  {
    if (m_ptr)
    {
      Assert(m_ptr->m_weakcount > 0);
      --(m_ptr->m_weakcount);
    }
  }
};
} 
