/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Factory.h,v $
Revision 1.6  2008/06/07 13:41:57  jay
MSVC rightly complains that we need <algorithm>

Revision 1.5  2008/05/09 18:03:10  jay
Work around gcc compile error, where iterator type not recognised

Revision 1.4  2007/09/02 09:14:13  jay
Temp fix for code which doesn't compile on Mac - luckily it's only needed
for Scene Ed. The compile error is no help, and it compiles OK in MSVC.
GCC doesn't like the map iterator declaration.

Revision 1.3  2007/08/29 22:21:30  jay
Add extra template for Key type; add function to list all Keys

Revision 1.2  2006/09/10 22:42:58  jay
Make Create() virtual

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

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
    FactoryFunction f = m_map[name];
    if (f)
    {
      return f();
    }
    return 0;
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
