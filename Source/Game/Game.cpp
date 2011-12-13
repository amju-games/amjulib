#include "Game.h"
#include "AmjuGL.h"
#include "Timer.h"
#include "EventPoller.h"
#include "Screen.h"

namespace Amju
{
Game::Game()
{
  TheEventPoller::Instance()->AddListener(this);
  m_currentState = 0;
  m_pauseState = 0;
  m_newState = 0;
}


void Game::PauseGame()
{
  if (m_pauseState)
  {
    SetCurrentState(m_pauseState);
  }
}

void Game::RegisterPauseState(GameState* pauseState)
{
  m_pauseState = pauseState;
}

void Game::Update()
{
  UpdateState();

  TheTimer::Instance()->Update();
  TheEventPoller::Instance()->Update();

  GetState()->Update();
}

void Game::Draw()
{
  AmjuGL::InitFrame();

  AmjuGL::BeginScene();

  // TODO Fix DX9 so we can have multple viewports
  AmjuGL::Viewport(0, 0, Screen::X(), Screen::Y());
    
  // Draw 3D Scene
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  GetState()->Draw();

  // Draw 2D Elements

  // Set matrices to identity
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();

  // NB Make sure we use MV matrix to transform cursor..?
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  GetState()->Draw2d();

  AmjuGL::EndScene();		
}

static bool s_quit = false;
bool Game::OnQuitEvent()
{
  s_quit = true;
  return false; // allow game states to override
}

void Game::Run()
{
  while (!s_quit)
  {
    RunOneLoop();
  }
}

void Game::RunOneLoop()
{
  Update();

  Draw();

  AmjuGL::Flip(); 
}

void Game::UpdateGameObjects()
{
  for (GameObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    it->second->Update();
  }
}

GameState* Game::GetState()
{
  Assert(m_currentState);
  return m_currentState;
}

void Game::SetCurrentState(GameState* gs)
{
  m_newState = gs;
}

void Game::UpdateState()
{
  if (!m_newState)
  {
    return;
  }

  if (m_currentState == m_newState)
  {
    m_newState = 0;
    return;
  }

  if (m_currentState)
  {
    TheEventPoller::Instance()->RemoveListener(m_currentState);
    m_currentState->OnDeactive();
  }

  m_currentState = m_newState;
  Assert(m_currentState);
  m_newState = 0;

  // This next line may set m_newState, so zero it first
  // TODO Not sure why, is this old ??
  m_currentState->OnActive();
  TheEventPoller::Instance()->AddListener(m_currentState);
}

PGameObject Game::GetGameObject(int id)
{
  // Allow IDs for non-existent game objects, but don't insert null ptr into map
  GameObjects::iterator it = m_objects.find(id);
  if (it == m_objects.end())
  {
    return 0;
  }  

  return it->second;
}

void Game::AddGameObject(PGameObject object)
{
  int id = object->GetId();
  Assert(m_objects.find(id) == m_objects.end());
  m_objects[id] = object;
}

void Game::EraseGameObject(int id)
{
  m_objects.erase(id);
}

void Game::ClearGameObjects()
{
  m_objects.clear();
}

GameObjects* Game::GetGameObjects()
{
  return &m_objects;
}
}
