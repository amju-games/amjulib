#include "AmjuHash.h"

namespace Amju
{
unsigned int HashString(const std::string& str)
{
  return std::hash<std::string>{}(str);

#ifdef OLD_SKOOL
  unsigned int hash = 5381;
  for (int c : str)
  {
    c = (c < 'A' || c > 'Z') ? c : (c - 'A' + 'a'); // Ignore case
    hash = ((hash << 5) + hash) + c;                // hash*33 + c
  }
  return hash;
#endif
}
}

