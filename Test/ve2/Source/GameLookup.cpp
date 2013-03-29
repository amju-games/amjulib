#include <AmjuFirst.h>
#include "GameLookup.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
std::string GameLookup(const std::string& text)
{
  Player* p =  GetLocalPlayer();
  Assert(p);

  std::string playerName = p->GetName();
  return Replace(text, "<p>", playerName);
}

}
