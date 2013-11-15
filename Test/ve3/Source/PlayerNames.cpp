#include <AmjuFirst.h>
#include <map>
#include "PlayerNames.h"
#include <AmjuFinal.h>

namespace Amju
{
typedef std::map<int, std::string> Names;
static Names names;

void SetPlayerName(int objId, const std::string& name)
{
  names[objId] = name;
}

const std::string& GetPlayerName(int objId)
{
  return names[objId];
}

}
