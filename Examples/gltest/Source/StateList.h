#pragma once

#include <map>
#include "GSBase.h"

namespace Amju
{
typedef std::map<std::string, GSBase*> StateList;

const StateList& GetStateList();

void AddState(const std::string& name, GSBase* state);

}

