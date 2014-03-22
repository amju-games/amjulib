/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Mouse.h,v $
Revision 1.2  2006/01/16 14:26:56  jay
Added flags for Right and middle buttons

Revision 1.1.10.1  2005/07/30 12:31:27  jay
Now holds state for Right- and middle-buttons

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(AMJU_MOUSE_H_INCLUDED)
#define AMJU_MOUSE_H_INCLUDED

#ifdef WIN32
#include <windows.h>
#endif

namespace Amju
{
struct Mouse
{
  // Mouse coords.
  static int s_mousex, s_mousey;
  // True if left mouse button is down.
  static bool s_mouseDown;
  // New for POOL: right and middle buttons too.
  static bool s_mouseRDown;
  static bool s_mouseMidDown;

  enum CursorType { STANDARD, HAND, FINGER };
  static void SetCursor(CursorType );

#ifdef WIN32
  static void SetInstance(HINSTANCE);
  static HINSTANCE s_hinst;
#endif
};
}

#endif

