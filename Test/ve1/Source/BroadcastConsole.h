#ifndef BROADCAST_CONSOLE_H_INCLUDED
#define BROADCAST_CONSOLE_H_INCLUDED

#include <deque>
#include <Singleton.h>
#include <GuiTextEdit.h>

namespace Amju
{
// Chat console visible to all players, overlaid on 3D scene.
// (Should have "hide/show" buttons?)
// Should just activate if you start typing. 
// Should work with soft KB.
// Need special msg recipient ID for broadcast.
class BroadcastConsole : public NonCopyable
{
private:
  BroadcastConsole();
  friend class Singleton<BroadcastConsole>;

public:
  void OnEnter();

  void Update();
  void Draw();

  // Called by MsgManager when we get a new broadcast msg
  void OnMsgRecv(const std::string&);

  void OnActive();

private:
  void ReposText();

private:
  typedef std::deque<RCPtr<GuiText> > Texts;
  Texts m_texts;
  RCPtr<GuiTextEdit> m_edit;
  float m_bottom; // bottom of window, taking KB into account
};

typedef Singleton<BroadcastConsole> TheBroadcastConsole;
}

#endif
