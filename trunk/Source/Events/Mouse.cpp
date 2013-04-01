/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include <iostream>
#include "Mouse.h"
#ifdef MACOSX_BUNDLE
#include <Carbon/Carbon.h>
#endif
#ifdef WIN32
#include <windows.h>
// This is a bit nasty :-(
#include "../WinMain/resource.h"
#endif
#include "AmjuAssert.h"
#include <AmjuFinal.h>

namespace Amju
{
int Mouse::s_mousex = -1;
int Mouse::s_mousey = -1;
bool Mouse::s_mouseDown = false;
bool Mouse::s_mouseRDown = false;
bool Mouse::s_mouseMidDown = false;

#ifdef WIN32
void Mouse::SetInstance(HINSTANCE hinst)
{
  AMJU_CALL_STACK;

  s_hinst = hinst;
}

HINSTANCE Mouse::s_hinst;
#endif

void Mouse::SetCursor(Mouse::CursorType ct)
{
  AMJU_CALL_STACK;

//
//
//   NB does nothing!
//
//

/*
#ifdef MACOSX_BUNDLE
  // TODO These IDs are allocated by ResEdit
  static CursHandle cursorHand = GetCursor(128);
  static CursHandle cursorFinger = GetCursor(129);
#endif 

#ifdef WIN32
  // TODO These IDs are just examples from MSDN
  Assert(s_hinst);
  static HCURSOR cursorHand = LoadCursor(s_hinst, MAKEINTRESOURCE(IDC_CURSOR1));
  static HCURSOR cursorFinger = LoadCursor(s_hinst, MAKEINTRESOURCE(IDC_CURSOR2));
  static HCURSOR cursorStandard = LoadCursor(0, IDC_ARROW);
#endif

  //std::cout << "Set cursor " << ct << "\n";

  static CursorType oldCt = STANDARD;

  //if (ct == oldCt)
  //{
  //  return;
  //}
 
  oldCt = ct;
  switch (ct)
  {
  case STANDARD:
#ifdef MACOSX_BUNDLE
    InitCursor();
#endif
#ifdef WIN32
    ::SetCursor(cursorStandard);
#endif
    break;

  case HAND:
#ifdef MACOSX_BUNDLE
    ::SetCursor(*cursorHand);
#endif
#ifdef WIN32
    ::SetCursor(cursorHand);
#endif
    break;

  case FINGER:
#ifdef MACOSX_BUNDLE
    ::SetCursor(*cursorFinger);
#endif
#ifdef WIN32
    ::SetCursor(cursorFinger);
#endif

    break;
  }
*/
}

}



