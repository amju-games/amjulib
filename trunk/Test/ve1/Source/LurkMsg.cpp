#include <Timer.h>
#include "LurkMsg.h"

namespace Amju
{
LurkMsg::LurkMsg()
{
  m_timer = 0;
  m_state = LURK_NEW;
  m_lurkPos = AMJU_NONE;
}

LurkMsg::LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp)
{
  Set(text, fgCol, bgCol, lp);
}

bool LurkMsg::IsFinished() const
{
  return m_state == LURK_FINISHED;
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

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;
  m_rect->SetLocalPos(m_pos);
  m_text->SetLocalPos(m_pos);

  m_rect->SetLocalPos(m_text->GetLocalPos());
}

void LurkMsg::Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp)
{
  m_lurkPos = lp;
  m_timer = 0;
  m_state = LURK_NEW;

  m_text = new GuiText;
  m_text->SetText(text);
  m_text->SizeToText();
  m_text->SetLocalPos(Vec2f(0, 0)); // TODO TEMP TEST
  m_text->SetFgCol(fgCol);

  m_rect = new GuiRect;
  m_rect->SetRoundCorners(12); // TODO TEMP TEST
  m_rect->SetSize(m_text->GetSize());
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(0.04f); // TODO TEMP TEST
}

void Lurker::Update()
{
  for (QMap::iterator it = m_qmap.begin(); it != m_qmap.end(); ++it)
  {
    LurkMsgQ& q = it->second;
    if (q.empty())
    {
      continue;
    }
    if (q.front().IsFinished())
    {
      q.pop();
    }
    else
    {
      q.front().Update();
    }
  }
}

void Lurker::Draw()
{
  for (QMap::iterator it = m_qmap.begin(); it != m_qmap.end(); ++it)
  {
    LurkMsgQ& q = it->second;
    if (q.empty())
    {
      continue;
    }
    q.front().Draw();
  }
}

void Lurker::Queue(const LurkMsg& lm)
{
  m_qmap[lm.m_lurkPos].push(lm);
}

}



