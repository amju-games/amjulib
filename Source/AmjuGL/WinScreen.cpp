#include <AmjuFirst.h>
#ifdef WIN32

#include "WinScreen.h"
#include <windows.h>
#include <AmjuFinal.h>

namespace Amju
{
static HWND s_hwnd = 0;

void* GetHWnd()
{
  return s_hwnd;
}

void SetHWnd(void* hwnd)
{
  s_hwnd = (HWND)hwnd;
}
}

#endif // WIN32

