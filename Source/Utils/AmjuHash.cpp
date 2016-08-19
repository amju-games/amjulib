#include "AmjuHash.h"

namespace Amju
{
  unsigned int HashString(const char* string)
  {
    unsigned int hash = 5381;
    int c;
    while ((c = *string++))
    {
      c = (c < 'A' || c > 'Z') ? c : (c - 'A' + 'a'); // Ignore case
      hash = ((hash << 5) + hash) + c;                // hash*33 + c
    }
    return hash;
  }

}

