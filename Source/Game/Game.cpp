#include <AmjuFirst.h>
#include "Game.h"
#include "AmjuGL.h"
#include "Timer.h"
#include "EventPoller.h"
#include "Screen.h"
#include <MessageQueue.h>
#include <StringUtils.h>
#include <ResourceManager.h>
#include <GuiText.h>
#include <Rect.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <AmjuFinal.h>
#endif

//#define STATE_DEBUG
#define SHOW_FRAME_TIME

namespace Amju
{
Game::Game()
{
  m_currentState = 0;
  m_pauseState = 0;
  m_newState = 0;
  m_font = 0;
  m_updateCopy = false;
}

void Game::SetUpdateCopy(bool uc)
{
  m_updateCopy = uc;
}

void Game::SetFrameTimeFont(Font* f)
{
  m_font = f;
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
  TheMessageQueue::Instance()->Update();

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

  // Turn off back face culling: for 2D GUI, everything should be
  //  front-facing, so we don't gain anything by having it on. 
  //  But turning it off lets us scale by negative numbers, to 
  //  flip/mirror 2D elements.
  AmjuGL::Disable(AmjuGL::AMJU_CULLING);

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
#ifdef SHOW_FRAME_TIME
    static std::string fps;
    static int framesThisSec = 0;
    framesThisSec++;
    static int elapsed = 0;
    float e = TheTimer::Instance()->GetElapsedTime(); 

    if ((int)e != elapsed)
    {
      elapsed = (int)e;
      fps = ToString(framesThisSec);
      framesThisSec = 0;
    }

#ifdef WIN32
    unsigned long start = GetTickCount();
#else
    // Get time taken to update/draw/flip, giving the 'real' FPS, not fixed to screen refresh rate
    timeval tbefore;
    gettimeofday(&tbefore, 0);
#endif

#endif //  SHOW_FRAME_TIME

  Update();

#ifdef SHOW_FRAME_TIME
#ifdef WIN32
    unsigned long mid = GetTickCount();
#else
    // Get time taken to update/draw/flip, giving the 'real' FPS, not fixed to screen refresh rate
    timeval mid;
    gettimeofday(&mid, 0);
#endif
#endif //  SHOW_FRAME_TIME

  Draw();

#ifdef SHOW_FRAME_TIME
  if (m_font)
  {
#ifdef WIN32
    unsigned long draw = GetTickCount() - mid;
    unsigned long update = mid - start;
    std::string s = "Draw: " + ToString((unsigned int)draw) + " update: " + ToString((unsigned int)update);

#else
    timeval tafter;
    gettimeofday(&tafter, 0);
    double draw = tafter.tv_sec - mid.tv_sec + (tafter.tv_usec - mid.tv_usec) * 1e-6;
    double update = mid.tv_sec - tbefore.tv_sec + (mid.tv_usec - tbefore.tv_usec) * 1e-6;
    int idraw = (int)(draw * 1000.0f);
    int iupdate = (int)(update * 1000.0f);
    std::string s = std::string("Draw: ") + 
      std::string((idraw < 10 ? "0" : "")) + ToString(idraw) +
      std::string("ms update: ") + std::string((iupdate < 10 ? "0" : "")) + 
      ToString(iupdate) + "ms";
#endif

    s += " fps: " + fps;

    // Display time per frame
    static GuiText t;
    t.SetFont(m_font);
    t.SetScaleX(0.7f);
    t.SetFgCol(Colour(1, 1, 1, 1));
    t.SetLocalPos(Vec2f(-1.0f, 1.0f));
    t.SetSize(Vec2f(2.0f, 0.1f));
    t.SetJust(GuiText::AMJU_JUST_LEFT);
    t.SetText(s);
    AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_TEXTURE_2D);
    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    t.Draw(); 
    AmjuGL::PopAttrib();
  }
#endif //  SHOW_FRAME_TIME

  AmjuGL::Flip(); 
}

void Game::UpdateGameObjects()
{
  // To be safe, we should copy the container of game objects, so
  //  any updates which add or delete game objects do not invalidate the
  //  iterator. But if this is expensive, leave it to the game-specific code
  //  to sort out...

  if (m_updateCopy)
  {
    GameObjects objs(m_objects); // Yikes, but updating might remove or add objects

    for (GameObjects::iterator it = objs.begin(); it != objs.end(); ++it)
    {
      it->second->Update();
    }
  }
  else
  {
    // Updates won't add or delete elements, so iterate over original container 
    for (GameObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      it->second->Update();
    }
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

    // Deactivating state will no longer get events
    Assert(TheEventPoller::Instance()->HasListener(m_listener));
    TheEventPoller::Instance()->RemoveListener(m_listener);

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

  // To avoid nasties, assume each new state does NOT have a modal listener set.
  TheEventPoller::Instance()->SetModalListener(nullptr); 

  // Add the new state as an event listener - this is better than doing it
  //  in GameState, where subclasses can forget to call the base class impl.
  m_listener = new GameStateListener(m_currentState);
  TheEventPoller::Instance()->AddListener(m_listener, m_currentState->GetEventPriority());

  // This next line may set m_newState, so zero it before - not after - this call!
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
