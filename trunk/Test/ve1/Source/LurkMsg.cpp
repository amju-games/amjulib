#include "LurkMsg.h"

namespace Amju
{
LurkMsg::LurkMsg()
{
  m_timer = 0;
}

void LurkMsg::Draw()
{
  m_rect->Draw();
  m_text->Draw();
}

void LurkMsg::Update()
{
  //Vec2f m_pos;
  //Vec2f m_vel;
}

void LurkMsg::Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp)
{
  m_text = new GuiText;
  m_text->SetText(text);
  m_text->SizeToText();
  m_text->SetLocalPos(Vec2f(0, 0)); // TODO TEMP TEST

  //enum LurkPos { AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT };

  m_rect = new GuiRect;
  m_rect->SetRoundCorners(12); // TODO TEMP TEST
  m_rect->SetSize(m_text->GetSize());
}
}



