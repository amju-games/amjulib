#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <SoundManager.h>
#include "GSStartMenu.h"
#include "GSChoosePlayer.h"
#include "GSStartGame.h"
#include "GameMode.h"
#include "GSTitle.h"
#include "GSAdminMenu.h"
#include "GSOptions.h"
#include "GSLoginWaiting.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnOptionsButton(GuiElement*)
{
  // Set back state!!
  TheGSOptions::Instance()->SetPrevState(TheGSStartMenu::Instance());
  TheGame::Instance()->SetCurrentState(TheGSOptions::Instance());
}

static void OnGuestButton(GuiElement*)
{
  TheGSLoginWaiting::Instance()->SetEmail("guest");
  TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
}

static void OnStartButton(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnCancelButton(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

static void OnAdminButton(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSAdminMenu::Instance());
}

GSStartMenu::GSStartMenu()
{
}

void GSStartMenu::Update()
{
  GSGui::Update();
}

void GSStartMenu::Draw()
{
  GSGui::Draw();
}

void GSStartMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSStartMenu::OnActive()
{
  SetGameMode(AMJU_MODE_NONE);

  GSGui::OnActive();

  m_gui = LoadGui("gui-startmenu.txt");
  Assert(m_gui);

  GuiButton* start = (GuiButton*)GetElementByName(m_gui, "start-button");
  start->SetCommand(Amju::OnStartButton);

  GuiButton* guest = (GuiButton*)GetElementByName(m_gui, "guest-button");
  // not supported currently 
  //guest->SetCommand(Amju::OnGuestButton);

  GuiButton* options = (GuiButton*)GetElementByName(m_gui, "options-button");
  options->SetCommand(Amju::OnOptionsButton);

  GuiButton* admin = (GuiButton*)GetElementByName(m_gui, "admin-button");
#ifdef _DEBUG
  admin->SetCommand(Amju::OnAdminButton);
#else
  admin->SetVisible(false);  
#endif

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  // TODO remember last choice and keep highlighted
  start->SetHasFocus(true); 

#ifdef PLAY_MUSIC
  TheSoundManager::Instance()->PlaySong("Sound/piano.it");
#endif
}

} // namespace
