/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ReportError.h"
#include "AmjuFinal.h"

namespace Amju
{
void ReportError(const std::string& error)
{
  AMJU_CALL_STACK;

  std::cout << error.c_str() << "\n"; 
  // TODO Log to file if enabled
}
}

