#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#ifdef WIN32

namespace Amju
{
// NB Use void* instead of HWND because if we include <windows.h>
//  lots of things get #defined which break our code.
void* GetHWnd();
void SetHWnd(void* hwnd);
}

#endif // WIN32
#endif // WIN_SCREEN_H

