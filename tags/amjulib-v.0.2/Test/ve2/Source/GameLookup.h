#ifndef GAME_LOOKUP_H_INCLUDED
#define GAME_LOOKUP_H_INCLUDED

#include <string>

namespace Amju
{
  // Do game-specific find and replace in a string:
  // <p> -> player name
  std::string GameLookup(const std::string& text);
}

#endif
