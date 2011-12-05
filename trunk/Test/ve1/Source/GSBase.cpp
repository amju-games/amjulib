#include "GSBase.h"
#include <Game.h>
#include <Timer.h>
#include <AmjuGL.h>
#include <iostream>
#include "ObjectManager.h"

namespace Amju
{
static float yrot = 0;

GSBase::GSBase() : m_time(0), m_maxTime(5.0f)
{
  m_nextState = 0;
}

void GSBase::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 5, 20,  0, 0, 0,  0, 1, 0);

  AmjuGL::RotateY(yrot);
}

void GSBase::Update()
{
  TheOnlineReqManager::Instance()->Update();

  TheObjectManager::Instance()->Update();

  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;

/*
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
*/
}

bool drag = false;

bool GSBase::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  if (drag)
  {
    float xdiff = ce.x - oldx;
    yrot += xdiff * 100.0f;
  }
  oldx = ce.x;
  return false;
}

bool GSBase::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    drag = mbe.isDown;  
  }
  return false;
}

}
