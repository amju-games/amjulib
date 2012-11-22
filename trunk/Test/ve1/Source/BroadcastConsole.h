#ifndef BROADCAST_CONSOLE_H_INCLUDED
#define BROADCAST_CONSOLE_H_INCLUDED

#include <Singleton.h>
#include <EventListener.h>

namespace Amju
{
// Chat console visible to all players, overlaid on 3D scene.
// (Should have "hide/show" buttons?)
// Should just activate if you start typing. 
// Should work with soft KB.
// Need special msg recipient ID for broadcast.
class BroadcastConsole : public NonCopyable, public EventListener
{
public:
  virtual bool OnKeyEvent(const KeyEvent& ke);

  void Update();
  void Draw();
};

typedef Singleton<BroadcastConsole> TheBroadcastConsole;
}

#endif
