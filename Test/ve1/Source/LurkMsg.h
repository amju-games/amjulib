#ifndef LURK_MSG_H_INCLUDED
#define LURK_MSG_H_INCLUDED

#include <string>
#include <Colour.h>
#include <GuiText.h>
#include <GuiRect.h>

namespace Amju
{
// Lurk on edge of screen
class LurkMsg
{
public:
  LurkMsg();

  enum LurkPos { AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT };

  void Draw();
  void Update();
  void Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp);
 
private:
  RCPtr<GuiText> m_text;
  RCPtr<GuiRect> m_rect;

  Vec2f m_pos;
  Vec2f m_vel;

  float m_timer;
};
}

#endif

