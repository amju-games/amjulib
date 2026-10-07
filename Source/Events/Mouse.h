// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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

