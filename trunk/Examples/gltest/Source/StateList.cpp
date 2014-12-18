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

}

