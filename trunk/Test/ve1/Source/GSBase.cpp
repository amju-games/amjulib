#include "GSBase.h"
#include <Game.h>
#include <Timer.h>
#include <AmjuGL.h>
#include <GuiText.h>
#include <iostream>
#include "ObjectManager.h"
#include "PosUpdate.h"
#include "GSNetError.h"

namespace Amju
{
GSBase::GSBase() : m_time(0), m_maxTime(5.0f)
{
  m_nextState = 0;
  m_drag = false;
  m_yrot = 0;
  m_prevState = 0;
}

void GSBase::OnActive()
{
  GameState::OnActive();

  // Set this state to be the previous state for the Error screen.
  // But not for the Error state!
  if (this != TheGSNetError::Instance())
  {
    TheGSNetError::Instance()->SetPrevState(this);
  }
}

void GSBase::SetPrevState(GameState* prevState)
{
  Assert(prevState != this);
  m_prevState = prevState;
}

GameState* GSBase::GetPrevState()
{
  return m_prevState;
}

void GSBase::SetNextState(GameState* next)
{
  m_nextState = next;
}

void GSBase::GoBack()
{
  if (!m_prevState)
  {
    // Not sure if using type_info like this works reliably :-(
    std::cout << "FAIL in Go back to prev state: current state: " << typeid(*this).name() << "\n"; 
  }
  Assert(m_prevState);

std::cout << "GO BACK to previous state...\n";

  TheGame::Instance()->SetCurrentState(m_prevState);
}

void GSBase::Draw2d()
{
}

void GSBase::Draw()
{
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

  AmjuGL::RotateY(m_yrot);
}

void GSBase::Update()
{
  // Handle any completed requests
  TheVe1ReqManager::Instance()->Update();

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
}
