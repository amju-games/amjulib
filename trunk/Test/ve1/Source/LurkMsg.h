#ifndef LURK_MSG_H_INCLUDED
#define LURK_MSG_H_INCLUDED

#include <queue>
#include <string>
#include <Colour.h>
#include <GuiText.h>
#include <GuiRect.h>
#include <Singleton.h>

namespace Amju
{
// Lurk on edge of screen
class LurkMsg
{
public:
  enum LurkPos { AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT };

  LurkMsg();
  LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp);


  void Draw();
  void Update();
  void Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp);
 
private:
  RCPtr<GuiText> m_text;
  RCPtr<GuiRect> m_rect;

  Vec2f m_pos;
  Vec2f m_vel;

  float m_timer;
  
  enum State { LURK_SHOWN, LURK_SHOWING, LURK_HIDDEN, LURK_HIDING };
  State m_state;
};

class Lurker : public NonCopyable
{
public:
  void Update();
  void Draw();
  void Queue(const LurkMsg& lm);

private:
  typedef std::queue<LurkMsg> m_q; 
};

typedef Singleton<Lurker> TheLurker;
}

#endif

