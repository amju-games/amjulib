#include "Game.h"
#include "AmjuGL.h"
#include "Timer.h"
#include "EventPoller.h"
#include "Screen.h"
#include <StringUtils.h>
#include <ResourceManager.h>
#include <Font.h>
#include <Rect.h>
#include <DrawRect.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

//#define AMJU_SHOW_FRAME_TIME

//#define STATE_DEBUG

namespace Amju
{
Game::Game()
{
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
#ifdef AMJU_SHOW_FRAME_TIME

#ifdef WIN32
  unsigned long start = GetTickCount();

#else
  // Get time taken to update/draw/flip, giving the 'real' FPS, not fixed to screen refresh rate
  timeval tbefore;
  gettimeofday(&tbefore, 0);

#endif
#endif


  Update();
  Draw();


#ifdef AMJU_SHOW_FRAME_TIME
  static Amju::Font* font =
    (Amju::Font*)Amju::TheResourceManager::Instance()->GetRes("font2d/arial-font.font");

  if (font)
  {
#ifdef WIN32
    unsigned long delta = GetTickCount() - start;
    std::string s = Amju::ToString((unsigned int)delta);

#else
    timeval tafter;
    gettimeofday(&tafter, 0);
    double t = tafter.tv_sec - tbefore.tv_sec + (tafter.tv_usec - tbefore.tv_usec) * 1e-6;
    std::string s = Amju::ToString((int)(t * 1000.0f));
#endif

    // Display time per frame
    s += "ms";
    PushColour();
    AmjuGL::SetColour(Colour(0, 0, 0, 1));
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    Rect r(-1.0f, -0.7f, -1.0f, -0.9f);
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
    font->Print(-1.0f, -1.0f, s.c_str());
  }

#endif //  AMJU_SHOW_FRAME_TIME

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
#ifdef STATE_DEBUG
std::cout << "Game::SetCurrentState: new state is " << typeid(*gs).name() << "\n";
#endif

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
#ifdef STATE_DEBUG
std::cout << "Game::UpdateState: deactivating state: " << typeid(*m_currentState).name() << "\n";
#endif

    m_currentState->OnDeactive();
  }
  else
  {
#ifdef STATE_DEBUG
std::cout << "Game::UpdateState: no current state to deactivate.\n";
#endif
  }

  m_currentState = m_newState;
  Assert(m_currentState);
  m_newState = 0;

#ifdef STATE_DEBUG
std::cout << "Game::UpdateState: activating new state: " << typeid(*m_currentState).name() << "\n";
#endif

  // This next line may set m_newState, so zero it first
  // TODO Not sure why, is this old ??
  m_currentState->OnActive();
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
