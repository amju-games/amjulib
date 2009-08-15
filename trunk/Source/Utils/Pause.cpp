#include "AmjuFirst.h"
#include "Pause.h"
#include "AmjuFinal.h"
#include <iostream>

#if defined(GEKKO) 
#include <wiiuse/wpad.h>
#endif // GEKKO


namespace Amju
{
void Pause(const char* funcName, const char* fileName, unsigned int line)
{
  AMJU_CALL_STACK;
  std::cout << "Paused in " << funcName << " (" << fileName << ":" << line << ")...\n";

#ifdef WIN32
  system("pause");
#endif // WIN32

#ifdef GEKKO

  WPAD_Init();
  // Wait forever, but repond to A button
  while (true)
  {
    WPAD_ScanPads();
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A) 
    {
      break; 
    }
    
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) 
    {
      exit(0); // Home - exit to HBC
    }    
  }

#endif
}
}
