#include <AmjuFirst.h>
#include "AmjuAssert.h"
#include <iostream>
#include <fstream>

#if defined(GEKKO) 
#include <Pause.h>
#include <AmjuTime.h>
#include <StringUtils.h>
#include <AmjuFinal.h>
#endif // GEKKO


namespace Amju
{
#if defined(GEKKO) 

void wiiAssert(const void* exp, const char* file, unsigned int line)
{
  AMJU_CALL_STACK;

  std::cout 
    << "Assertion failed: "
    << (const char *)exp
    << std::endl
    << "File: "
    << file
    << "\nLine: "
    << line
    << std::endl;

  std::ofstream of;
  of.open("/assertlog.txt", std::ios::out | std::ios::app);
  if (of.good()) 
  {
    of
      << Time::Now().ToString() << ": expr: \""
      << (const char *)exp
      << "\" File: "
      << StripPath(file)
      << " Line: "
      << line
      << "\r\n";
    /*
      << "Callstack: ";
    const CallStack::FunctionNames& fn = CallStack::Instance()->GetFunctionNames();
    for (unsigned int i = 0; i < fn.size(); i++)
    {
      of << fn[i] << " ";
    }
    of << std::endl;
    */
  }
  else
  {
    std::cout << "Failed to open log file for writing :-(\n";
  }
  PAUSE

  /*
  WPAD_Init();
  // Wait forever, but repond to home/power/reset
  while (true)
  {
    WPAD_ScanPads();
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) 
    {
      // Home
      exit(0);
    }
  }
  */
}
#endif // GEKKO

#if defined(_DEBUG) && defined(WIN32)

void winAssert(const void* exp, const void* file, unsigned line)
{
  std::cout 
    << "Assertion failed: "
    << (const char *)exp
    << std::endl
    << "File: "
    << (const char *)file
    << " line: "
    << line
    << std::endl;

  __asm
  {
    int 3 // break
  }
}
#endif // _DEBUG & WIN32

}
