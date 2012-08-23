#include <Timer.h>
#include <EventPoller.h>
#include "Kb.h"
#include "ChatConsole.h"

namespace Amju
{
Kb::Kb()
{
  m_mode = KB_HIDDEN;

  // Decide whether to enable or not. Some platforms must have soft kb -
  //  otherwise it's a user pref.

  SetEnabled(false); 
}

bool Kb::Load(const std::string& guiKbFilename)
{
  // Remove old listener ?
  Deactivate();

  m_kb = new GuiKeyboard;
  return m_kb->OpenAndLoad(guiKbFilename); //"gui-kb-2.txt");
}

void Kb::SetEnabled(bool b)
{
  m_enabled = b;
}

bool Kb::IsEnabled() const
{
  return m_enabled;
}

void Kb::Update()
{
  if (!m_enabled) 
  {
    return;
  }

  Assert(m_kb); // call Load first

  static const float vel = 10.0f;
  float ypos = -1 + m_kb->GetSize().y;

  switch (m_mode)
  {
  case KB_SHOWN:
    m_kb->SetVisible(true);
    break;

  case KB_HIDDEN:
    break;

  case KB_SHOWING:
    {
    m_kb->SetVisible(true);
    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_kb->GetLocalPos();
    pos.y += vel * dt;
    if (pos.y > ypos)
    {
      pos.y = ypos;
      m_mode = KB_SHOWN;
    }
    m_kb->SetLocalPos(pos);
    }
    break;

  case KB_HIDING:
    {
    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_kb->GetLocalPos();
    pos.y -= vel * dt;
    if (pos.y < -1.0f)
    {
      m_mode = KB_HIDDEN;
      m_kb->SetVisible(false);
    }
    m_kb->SetLocalPos(pos);
    }
    break;
  }
}

void Kb::Draw()
{
  if (m_enabled)
  {
    m_kb->Draw();
  }
}

void Kb::Activate()
{
  if (!m_enabled)
  {
    return;
  }

  Assert(m_kb); // call Load first

  Vec2f pos = m_kb->GetLocalPos();
  pos.y = -1.0f;
  m_kb->SetLocalPos(pos);
 

  m_mode = KB_SHOWING;

  TheEventPoller::Instance()->AddListener(m_kb);
}

void Kb::Deactivate()
{
/*
  if (!m_enabled)
  {
    return;
  }
*/

  m_mode = KB_HIDING;
  static EventPoller* e = TheEventPoller::Instance();

  if (m_kb && e->HasListener(m_kb))
  {
    e->RemoveListener(m_kb);
  }
}

}

