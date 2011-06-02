#include "GSBase.h"
#include <Game.h>
#include <Timer.h>

namespace Amju
{
GSBase::GSBase() : m_time(0), m_maxTime(5.0f)
{
}

void GSBase::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;
  if (m_time >= m_maxTime)
  {
    m_time = 0;
    TheGame::Instance()->SetCurrentState(m_nextState);
  }
}

}
