#include <AmjuFirst.h>
#include "Factory.h"
#include <iostream>
#include "SharedPtr.h"
#include <AmjuFinal.h>

namespace Amju
{
class A : public Shareable
{
public:
  virtual ~A() {}
  virtual void print() { std::cout << "A"; }
};

class B : public A 
{
public:
  virtual void print(){ std::cout << "B"; }
};


A* CreateA()
{
  return new A;
}
}

using namespace Amju;

int main()
{
  AMJU_CALL_STACK;

  Factory<A> afactory;
  afactory.RegisterFactoryFunction("a", &CreateA);
  
  std::vector<std::string> strs = afactory.GetTypeNames();


  A* pMyObject = afactory.Create("b");
  pMyObject->print();
  std::cout << "\n";

  return 0;
}

