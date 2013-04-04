#include <math.h>
#include "GSLogo.h"
#include "Game.h"
#include "Timer.h"
#include "Colour.h"
#include "GSTitle.h"
#include "SoundManager.h"
#include "ResourceManager.h"

namespace Amju
{
//static const char* GROUP = "logo-group";

const char* GSLogo::NAME = "logo";

void Done()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

GSLogo::GSLogo()
{
  m_fade = 0;
}

void GSLogo::OnActive()
{
  GameState::OnActive();
//  TheResourceManager::Instance()->LoadResourceGroup(GROUP);
  m_image.OpenAndLoad("logo-gui.txt");
}

void GSLogo::OnDeactive()
{
  GameState::OnDeactive();
//  TheResourceManager::Instance()->FreeResourceGroup(GROUP);
}

void GSLogo::Update()
{
  m_fade += TheTimer::Instance()->GetDt();
}

void GSLogo::Draw()
{
}

void GSLogo::Draw2d()
{
  PushColour();
  float s = sin(m_fade);
  if (s < 0)
  {
    s = 0;
    Done();
  }
  MultColour(Colour(s, s, s, 1.0f));
  m_image.Draw();
  PopColour();
}

bool GSLogo::OnKeyEvent(const KeyEvent& ke)
{
  Done();
  return true;
}
}
