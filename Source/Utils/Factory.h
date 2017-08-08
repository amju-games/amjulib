// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#ifndef FACTORY_H_INCLUDED
#define FACTORY_H_INCLUDED

#include <algorithm> // for_each
#include <map>
#include <string>
#include <vector>

#include <iostream>

namespace Amju
{

// Creates objects of type T or subclass-of-T.
// Each class must be registered with the Factory, by calling
// RegisterFactoryFunction. The function that is registered
// should return a new object.
// This Factory design comes from "Modern C++ Design", but MSVC
// can't compile the Loki library, so this is a simplified version.
// Class S is the type used to identify the subclass of T you want.
template <class T, class S = std::string>
class Factory 
{
public:
  // Function which creates a new T.
  typedef T* (*FactoryFunction)();

  typedef std::map<S, FactoryFunction> FuncMap;

  // Called once for each subtype of T. Registers a function
  // which creates an object of that type.
  bool Add(const S& name, FactoryFunction f)
  {
    m_map[name] = f;
    return true;
  }

  // Create an object of type T. The subtype of
  // the object depends on the string. This should match the ID
  // of a subtype which is registered.
  // Override this to create objects which aren't registered.
  T* Create(const S& name)
  {
    auto it = m_map.find(name);
    if (it == m_map.end())
      return nullptr;
    
    FactoryFunction f = it->second;
    if (f)
      return f();
    
    return nullptr;
  }

  struct Pusher
  {
    Pusher(std::vector<S>* pVec) : m_pVec(pVec) {}
    void operator() (std::pair<S, FactoryFunction> p)
    {
      m_pVec->push_back(p.first);
    }
    std::vector<S>* m_pVec;
  };

  std::vector<S> GetTypeNames() const
  {
    std::vector<S> s;
    std::for_each(m_map.begin(), m_map.end(), Pusher(&s));
    return s;
  }

protected:
  FuncMap m_map;
};
}
#endif
