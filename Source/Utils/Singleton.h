/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#if !defined(SINGLETON_T_H_INCLUDED)
#define SINGLETON_T_H_INCLUDED

#include "AmjuFirst.h"
#ifdef _DEBUG
#include <stdlib.h>
#include <iostream>
#include <typeinfo>
#endif
#include <NonCopyable.h>
#include <stdlib.h> // atexit
#include "AmjuFinal.h"

//#define SINGLETON_DEBUG

namespace Amju
{
template <class T>
class Singleton : public NonCopyable
{
public:
  static void Destroy()
  {
#ifdef SINGLETON_DEBUG
std::cout << "Destroying singleton: " << typeid(T).name() << "\n";
#endif

    delete Instance();
  }

  static T* Instance()
  {
    AMJU_CALL_STACK;

    static T* t = 0;
    if (!t)
    {
#ifdef SINGLETON_DEBUG
std::cout << "Creating singleton: " << typeid(T).name() << "\n";
#endif

      t = new T;
      atexit(Destroy);
    }
    return t;
  }
};

template <class T>
class SingletonNoDestroy : public NonCopyable
{
public:
  static T* Instance()
  {
    AMJU_CALL_STACK;

    static T* t = 0;
    if (!t)
    {
#ifdef SINGLETON_DEBUG
std::cout << "Creating singleton: " << typeid(T).name() << "\n";
#endif

      t = new T;
    }
    return t;
  }
};
}
#endif
