#include "StateList.h"

namespace Amju
{
static StateList theStatelist;

const StateList& GetStateList()
{
  return theStatelist;
}

void AddState(const std::string& name, GSBase* state)
{
  theStatelist[name] = state;
}

GSBase* GetState(int listboxSelection)
{
  auto it = theStatelist.begin();
  std::advance(it, listboxSelection);

  std::cout << "Selecting state '" << it->first << "'...\n";
  GSBase* gs = it->second;
  return gs;
}

}

