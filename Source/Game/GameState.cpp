#include <iostream>
#include <typeinfo>
#include <AmjuFirst.h>
#include <GameStateListener.h>
#include "Game.h"
#include "GameState.h"
#include "EventPoller.h"
#include <AmjuFinal.h>

namespace Amju
{
GameState::GameState() : 
  m_prevState(nullptr), 
  m_eventPriority(1),
  m_listener(nullptr)
{
}

GameState::~GameState()
{
}

void GameState::OnActive()
{
  m_listener = new GameStateListener(this);
  TheEventPoller::Instance()->AddListener(m_listener, m_eventPriority);
}

void GameState::OnDeactive()
{
  TheEventPoller::Instance()->RemoveListener(m_listener);
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
