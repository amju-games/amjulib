#ifndef LURK_MSG_H_INCLUDED
#define LURK_MSG_H_INCLUDED

#include <map>
#include <queue>
#include <string>
#include <Colour.h>
#include <GuiText.h>
#include <GuiRect.h>
#include <Singleton.h>

namespace Amju
{
enum LurkPos { AMJU_NONE, AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT, AMJU_CENTRE };

// Lurk on edge of screen
class LurkMsg
{
public:
  LurkMsg();
  LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp);

  void Draw();
  void Update();
  void Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp);

  bool IsFinished() const;
 
private:
  RCPtr<GuiText> m_text;
  RCPtr<GuiRect> m_rect;

  Vec2f m_pos; // current pos
  Vec2f m_finalPos; // position when LURK_SHOWN state is reached
  Vec2f m_vel;

  float m_timer;
  
  enum State { LURK_NEW, LURK_SHOWN, LURK_SHOWING, LURK_HIDDEN, LURK_HIDING, LURK_FINISHED };
  State m_state;

  friend class Lurker;
  LurkPos m_lurkPos;
};

class Lurker : public NonCopyable
{
public:
  void Update();
  void Draw();
  void Queue(const LurkMsg& lm);

private:
  typedef std::queue<LurkMsg> LurkMsgQ; // queue of msgs or one Lurk pos
  typedef std::map<LurkPos, LurkMsgQ> QMap; 
  QMap m_qmap; // one queue for each position
};

typedef Singleton<Lurker> TheLurker;
}

#endif

