#include <AmjuFirst.h>
#include "Game.h"
#include "GameState.h"
#include "EventPoller.h"
#include <AmjuFinal.h>

namespace Amju
{
void GameState::OnActive()
{
}

void GameState::OnDeactive()
{
}

void GameState::SetPrevState(GameState* prevState)
{
  Assert(prevState != this);
  m_prevState = prevState;
}

GameState* GameState::GetPrevState()
{
  return m_prevState;
}

void GameState::GoBack()
{
  if (!m_prevState)
  {
    // Not sure if using type_info like this works reliably :-(
    std::cout << "FAIL in Go back to prev state: current state: " << typeid(*this).name() << "\n"; 
  }
  Assert(m_prevState);

#ifdef _DEBUG
std::cout << "GO BACK to previous state... (" << typeid(*m_prevState).name() << ")\n";
#endif

  TheGame::Instance()->SetCurrentState(m_prevState);
}

}
