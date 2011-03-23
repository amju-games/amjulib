#ifndef EVENT_POLLER_IMPL_WIN32
#define EVENT_POLLER_IMPL_WIN32

#include "EventPoller.h"
#include <windows.h>

namespace Amju
{
class EventPollerImplWin32 : public EventPollerImpl
{
public:
  EventPollerImplWin32();
  ~EventPollerImplWin32();
  virtual void Update(Listeners* pListeners);

  static LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

private:
  void MouseMove(int x, int y);
  void MouseButton(Amju::MouseButton button, bool down);
  void OnKey(WPARAM c, bool down);

private:
  Listeners* m_listeners;
  CursorEvent m_ce; // store last coords

  // Pointer to one and only instance
  static EventPollerImplWin32* s_instance;
};
}

#endif
