#include <iostream>
#include <Game.h>
#include <Timer.h>
#include <AmjuGL.h>
#include <GuiText.h>
#include <StringUtils.h>
#include "GSBase.h"

namespace Amju
{
static const std::string KEYS = "[P]ause [S]kip [R]eset ";

GSBase::GSBase() : m_time(0), m_maxTime(5.0f), m_paused(false)
{
}

std::string GSBase::GetShaderDir() const
{
#ifdef AMJU_IOS 
  return "gles";
#endif

  return "opengl";

  // TODO DX9, DX11
}

void GSBase::OnActive() 
{
  m_time = 0;
}

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'r')
  {
    std::cout << "Reset!\n";
    OnActive();
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'p')
  {
    m_paused = !m_paused;
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 's')
  {
    m_time = m_maxTime;
  }
  return false;
}
 
void GSBase::Draw2d()
{
  static GuiText s;
  s.SetJust(GuiText::AMJU_JUST_LEFT);
  s.SetFgCol(Colour(1, 1, 1, 1));
  s.SetBgCol(Colour(0, 0, 0, 1));
//  s.SetDrawBg(true);
  s.SetLocalPos(Vec2f(-1, 0.9f));
  s.SetSize(Vec2f(2, 0.1f));
  s.SetText(KEYS + m_name + " " + ToString(m_maxTime - m_time, 1));
  s.Draw();
}

void GSBase::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f;
  const float FAR = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 5, 10,  0, 0, 0,  0, 1, 0);
}

void GSBase::Update()
{
  if (m_paused)
  {
    return;
  }

  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;
  if (m_time >= m_maxTime)
  {
    m_time = 0;
    if (!m_nextState)
    {
      std::cout << "Oops, no next state set\n";
      Assert(0);
    }
    TheGame::Instance()->SetCurrentState(m_nextState);
  }
}

}
