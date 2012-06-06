#include "GSStartMenu.h"
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSChoosePlayer.h"
#include "GSStartGame.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"
#include "GSTitle.h"
#include "GSAdminMenu.h"

namespace Amju
{
static void OnNoGameButton()
{
  SetGameMode(AMJU_MODE_NO_GAME); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnSingleButton()
{
  // TODO Still connect to server to log activity and get run time content.
  // Set single player flag.
  SetGameMode(AMJU_MODE_SINGLE); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnMultiButton()
{
  SetGameMode(AMJU_MODE_MULTI); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

static void OnAdminButton()
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
  GSGui::OnActive();

  m_gui = LoadGui("gui-startmenu.txt");
  Assert(m_gui);

  GuiButton* single = (GuiButton*)GetElementByName(m_gui, "single-button");
  single->SetIsEnabled(false);
  single->SetCommand(Amju::OnSingleButton);

  GuiButton* multi = (GuiButton*)GetElementByName(m_gui, "multi-button");
  multi->SetCommand(Amju::OnMultiButton);

  GuiButton* nogame = (GuiButton*)GetElementByName(m_gui, "nogame-button");
  nogame->SetIsEnabled(false);
  nogame->SetCommand(Amju::OnNoGameButton);

  GetElementByName(m_gui, "admin-button")->SetCommand(Amju::OnAdminButton);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  // TODO remember last choice and keep highlighted
  multi->SetHasFocus(true); 
}

} // namespace
