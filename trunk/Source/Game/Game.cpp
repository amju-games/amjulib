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
  AmjuGL::InitFrame(0, 0, 0);

  AmjuGL::BeginScene();

	// TODO We need to be able to get the preferred/allowed viewport dimensions ?!
	// x, y, w, h -- w and h are full screen if zero
  // TODO Fix DX9 so we can have multple viewports
  AmjuGL::Viewport(0, 0, Screen::X(), Screen::Y());
    
  // Set perspective 
  // TODO We need to be able to get the viewport dimensions ?!
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  // No need to set identity, this call overwrites proj matrix
  const float FOVY = 60.0f;
  const float aspect = (float)Screen::X() / (float)Screen::Y();
  const float NEAR = 1.0f; 
  const float FAR = 3000.0f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  // Draw 3D Scene

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
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
void Game::OnQuitEvent()
{
  s_quit = true;
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

GameState* Game::GetState(const char* stateName)
{
  return m_states[stateName];
}

void Game::SetCurrentState(const char* stateName)
{
  GameStates::iterator it = m_states.find(stateName);
  Assert(it != m_states.end());
  m_newState = it->second;
}

void Game::UpdateState()
{
  if (!m_newState)
  {
    return;
  }

  if (m_currentState == m_newState)
  {
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

bool Game::AddState(const char* stateName, PGameState state)
{
  m_states[stateName] = state;
  return true;
}

PGameObject Game::GetGameObject(int id)
{
  return m_objects[id];
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

Game::GameObjects* Game::GetGameObjects()
{
  return &m_objects;
}
}
