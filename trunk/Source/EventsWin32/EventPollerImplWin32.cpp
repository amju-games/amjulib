#include "EventPollerImplWin32.h"
#include <windows.h>
#include <WinScreen.h>
#include <Screen.h>

namespace Amju
{
static  bool active = true;

EventPollerImplWin32* EventPollerImplWin32::s_instance = 0;

EventPollerImplWin32::EventPollerImplWin32()
{
  m_listeners = 0;
  Assert(!s_instance);
  s_instance = this;
}

EventPollerImplWin32::~EventPollerImplWin32()
{
  s_instance = 0;
}

void EventPollerImplWin32::OnKey(WPARAM c, bool down)
{
  KeyEvent ke;
  ke.keyDown = down;
  switch (c)
  {
  case VK_UP:
    ke.keyType = AMJU_KEY_UP;
    break;
  case VK_DOWN:
    ke.keyType = AMJU_KEY_DOWN;
    break;
  case VK_LEFT:
    ke.keyType = AMJU_KEY_LEFT;
    break;
  case VK_RIGHT:
    ke.keyType = AMJU_KEY_RIGHT;
    break;
  case VK_SPACE:
    ke.keyType = AMJU_KEY_SPACE;
    break;
  case VK_RETURN:
    ke.keyType = AMJU_KEY_ENTER;
    break;
  case VK_ESCAPE:
    ke.keyType = AMJU_KEY_ESC;
    break;
  default:
    // TODO character
    ke.keyType = AMJU_KEY_CHAR;
    //ke.key = ???
  }

  if (m_listeners)
  {
    for (Listeners::iterator it = m_listeners->begin(); it != m_listeners->end(); ++it)
    {
      EventListener* e = *it;
      Assert(e);
      e->OnKeyEvent(ke);
    }
  }
}

void EventPollerImplWin32::MouseButton(Amju::MouseButton button, bool down)
{
  MouseButtonEvent mbe;
  mbe.button = button;
  mbe.isDown = down;
  mbe.x = m_ce.x;
  mbe.y = m_ce.y;

  if (m_listeners)
  {
    for (Listeners::iterator it = m_listeners->begin(); it != m_listeners->end(); ++it)
    {
      EventListener* e = *it;
      Assert(e);
      e->OnMouseButtonEvent(mbe);
    }
  }
}

void EventPollerImplWin32::MouseMove(int x, int y)
{
  if (m_listeners)
  {
    m_ce.controller = 0;
    m_ce.x = (float)x / (float)Screen::X() * 2.0f - 1.0f;
    m_ce.y = 1.0f - (float)y / (float)Screen::Y() * 2.0f;

    for (Listeners::iterator it = m_listeners->begin(); it != m_listeners->end(); ++it)
    {
      EventListener* e = *it;
      Assert(e);
      e->OnCursorEvent(m_ce);
    }
  }
}

LRESULT EventPollerImplWin32::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg) 
  {
  case WM_MOUSEMOVE:
    if (s_instance)
    {
      int x = LOWORD(lParam); 
      int y = HIWORD(lParam); 
      s_instance->MouseMove(x, y);
    }
    break;
 
  case WM_LBUTTONDOWN:
    if (s_instance)
    {
      // SetCapture() means we still get mouse events when the
      // mouse leaves the client area.
      SetCapture((HWND)GetHWnd());
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_LEFT, true);
    }
    break;

  case WM_LBUTTONUP:
    if (s_instance)
    {
      // We must now release the mouse - end of SetCapture()
      // mouse ownership.
      ReleaseCapture();
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_LEFT, false);
    }
    break;

  case WM_RBUTTONDOWN:
    if (s_instance)
    {
      SetCapture((HWND)GetHWnd());
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_RIGHT, true);
    }
    break;

  case WM_RBUTTONUP:
    if (s_instance)
    {
      ReleaseCapture();
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_RIGHT, false);
    }
    break;

  case WM_MBUTTONDOWN:
    if (s_instance)
    {
      SetCapture((HWND)GetHWnd());
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_MIDDLE, true);
    }
    break;

  case WM_MBUTTONUP:
    if (s_instance)
    {
      ReleaseCapture();
      s_instance->MouseButton(AMJU_BUTTON_MOUSE_MIDDLE, false);
    }
    break;

  case WM_KEYDOWN:
    if (s_instance)
    {
      s_instance->OnKey(wParam, true);
    }
    break;

  case WM_KEYUP:
    if (s_instance)
    {
      s_instance->OnKey(wParam, false);
    }
    break;

  case WM_CLOSE:
    exit(0); // TODO
    //PostQuitMessage(0);
    return 0;

  case WM_QUIT:
    exit(0); // TODO
    return 0;

  case WM_SYSCOMMAND:
    if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
    {
      return 0;
    }
    // Fall through

  default:
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
  }

  return 0;
}

void EventPollerImplWin32::Update(Listeners* pListeners)
{
  m_listeners = pListeners;

  int bGotMsg;
  MSG  msg;

  PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
//  while (msg.message != WM_QUIT) 
  {
    // Use PeekMessage() if the app is active, so we can use idle time to
    // render the scene. Else, use GetMessage() to avoid eating CPU time.
    bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

    if (bGotMsg) 
    {
      // Translate and dispatch the message
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    } 
    else 
    {
      // Not necessary, as we are in the Game main loop
      /*
      if (active) // TODO TEMP TEST
      {
        HWND hWnd = (HWND)GetHWnd();
        Assert(hWnd);
        InvalidateRect(hWnd, 0, FALSE);
      }
      */
    }
  }
}
}