#include <Timer.h>
#include "LurkMsg.h"

namespace Amju
{
static const float LURK_MAX_TIME = 3.0f;

// Extra border around text
static const Vec2f EXTRA(0.1f, 0.05f); // TODO CONFIG

LurkMsg::LurkMsg()
{
  m_timer = 0;
  m_state = LURK_NEW;
  m_lurkPos = AMJU_NONE;
  m_scale = 1.0f;
}

LurkMsg::LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  CommandFunc onFinished)
{
  Set(text, fgCol, bgCol, lp, onFinished);
}

bool LurkMsg::IsFinished() const
{
  return m_state == LURK_FINISHED;
}

void LurkMsg::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::Scale(m_scale, m_scale, 1.0f);
  m_rect->Draw();
  m_text->Draw();
  AmjuGL::PopMatrix();
}

void LurkMsg::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  switch (m_state)
  {
  case LURK_NEW:
    m_state = LURK_SHOWING;
    m_rect->SetVisible(true);
    m_text->SetVisible(true);
    break;

  case LURK_SHOWN:
    m_timer += dt;
    if (m_timer > LURK_MAX_TIME)
    {
      m_state = LURK_HIDING;
    }
    break;

  case LURK_SHOWING:
    // Fully showing ?
    if ((m_lurkPos == AMJU_TOP    && m_pos.y < m_showPos.y) ||
        (m_lurkPos == AMJU_BOTTOM && m_pos.y > m_showPos.y) ||
        (m_lurkPos == AMJU_LEFT   && m_pos.x > m_showPos.x) ||
        (m_lurkPos == AMJU_RIGHT  && m_pos.x < m_showPos.x))
    {
      m_rect->SetLocalPos(m_showPos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y));
      m_text->SetLocalPos(m_showPos);
      m_state = LURK_SHOWN;
    }
    else if (m_lurkPos == AMJU_CENTRE && m_scale > 1.0f)
    {
      m_scale = 1.0f;
      m_state = LURK_SHOWN;
    }
    else 
    {
      Vec2f  dpos = m_vel * dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

      m_scale += 2.0f * dt; // TODO TEMP TEST
    }
    break;

  case LURK_HIDING: 
    // Fully hidden ?
    if ((m_lurkPos == AMJU_TOP    && m_pos.y > m_hidePos.y) ||
        (m_lurkPos == AMJU_BOTTOM && m_pos.y < m_hidePos.y) ||
        (m_lurkPos == AMJU_LEFT   && m_pos.x < m_hidePos.x) ||
        (m_lurkPos == AMJU_RIGHT  && m_pos.x > m_hidePos.x) ||
        (m_lurkPos == AMJU_CENTRE && m_scale < 0.25f))
    {
      m_state = LURK_FINISHED;
      m_scale = 0;

      if (m_onFinished)
      {
        m_onFinished();
      }
    }
    else
    {
      Vec2f  dpos = m_vel * -dt;
      m_pos += dpos;
      m_rect->SetLocalPos(m_rect->GetLocalPos() + dpos);
      m_text->SetLocalPos(m_text->GetLocalPos() + dpos);

      m_scale -= 2.0f * dt; // TODO TEMP TEST
    }
    break;

  case LURK_FINISHED:
    m_rect->SetVisible(false);
    m_text->SetVisible(false);
    break;
  }
}

void LurkMsg::Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
  CommandFunc onFinished)
{
  m_lurkPos = lp;
  m_timer = 0;
  m_state = LURK_NEW;
  m_onFinished = onFinished;

  m_text = new GuiText;
  if (lp == AMJU_CENTRE)
  {
    m_text->SetIsMulti(true);
  }
  m_text->SetTextSize(1.5f); // TODO CONFIG
  m_text->SetSize(Vec2f(1.6f, 0.1f)); // assume single line
  m_text->SetText(text);
  m_text->SizeToText();
  m_text->SetFgCol(fgCol);

  m_rect = new GuiRect;
  m_rect->SetSize(m_text->GetSize() + EXTRA);
  m_rect->SetColour(bgCol);
  m_rect->SetCornerRadius(0.04f); // TODO CONFIG

  float h = m_text->GetSize().y;
  float w = m_text->GetSize().x;

  switch (m_lurkPos)
  {
  case AMJU_TOP:
    m_showPos = Vec2f(m_rect->GetSize().x * -0.5f, 1.0f);
    m_hidePos = Vec2f(m_showPos.x, 1.0f + h + EXTRA.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_TL | GuiRect::AMJU_TR);
    break;

  case AMJU_BOTTOM:
    m_showPos = Vec2f(m_rect->GetSize().x * -0.5f, -1.0f + h);
    m_hidePos = Vec2f(m_showPos.x, -1.0f - EXTRA.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BL | GuiRect::AMJU_BR);
    break;

  case AMJU_LEFT:
    m_showPos = Vec2f(-1.0f, h * 0.5f);
    m_hidePos = Vec2f(-1.0f - w - EXTRA.x, m_showPos.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BL | GuiRect::AMJU_TL);
    break;

  case AMJU_RIGHT:
    m_showPos = Vec2f(1.0f - w, h * 0.5f);
    m_hidePos = Vec2f(1.0f + EXTRA.x, m_showPos.y);
    m_rect->SetRoundCorners(GuiRect::AMJU_BR | GuiRect::AMJU_TR);
    break;

  case AMJU_CENTRE:
    m_showPos = Vec2f(-w * 0.5f, h * 0.5f - 0.3f); // TODO CONFIG y-offset
    m_hidePos = m_showPos;
    m_rect->SetRoundCorners(0); 
    m_scale = 0.5f;
    break;
  }

  m_vel = (m_showPos - m_hidePos);
  if (m_vel.SqLen() > 0)
  {
    m_vel.Normalise();
    m_vel *= 0.5f; // TODO CONFIG
  }
  m_pos = m_hidePos;
  m_text->SetLocalPos(m_hidePos); 
  m_rect->SetLocalPos(m_hidePos - 0.5f * Vec2f(EXTRA.x, -EXTRA.y)); 
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

void Lurker::Clear()
{
  m_qmap.clear();
}

}



