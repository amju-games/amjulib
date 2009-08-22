#include "AmjuFirst.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"
#include <iostream>
#include <fstream>

#if defined(GEKKO) 
#include <wiiuse/wpad.h>
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
  of.open("/assertlog.txt", std::ios::app);
  if (of.good()) 
  {
    of
      << (const char *)exp
      << std::endl
      << "File: "
      << file
      << "\nLine: "
      << line
      << std::endl
      << "Callstack: ";
    const CallStack::FunctionNames& fn = CallStack::Instance()->GetFunctionNames();
    for (unsigned int i = 0; i < fn.size(); i++)
    {
      of << fn[i] << " ";
    }
    of << std::endl;
  }

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
