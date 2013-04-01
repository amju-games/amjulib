/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include <AmjuFirst.h>
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string.h>
#include "SafeUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
bool SafeStrCmp(const char* s1, const char* s2)
{
  AMJU_CALL_STACK;

  if (!s1 || !s2)
  {
    return false;
  }
  return (strcmp(s1, s2) == 0);
}
}

