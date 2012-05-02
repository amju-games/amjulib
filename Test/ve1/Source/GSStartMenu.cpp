#include "GSStartMenu.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSChoosePlayer.h"
#include "GSStartGame.h"
#include "GameMode.h"

namespace Amju
{
void OnNoGameButton()
{
  SetGameMode(AMJU_MODE_NO_GAME); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

void OnLocalButton()
{
  // Local mode -- no server connection
  SetGameMode(AMJU_MODE_LOCAL); 
  //TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

void OnSingleButton()
{
  // TODO Still connect to server to log activity and get run time content.
  // Set single player flag.
  SetGameMode(AMJU_MODE_SINGLE); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

void OnMultiButton()
{
  SetGameMode(AMJU_MODE_MULTI); 
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

void OnEditButton()
{
  SetGameMode(AMJU_MODE_EDIT);
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
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

  GetElementByName(m_gui, "single-button")->SetCommand(Amju::OnSingleButton);
  GetElementByName(m_gui, "multi-button")->SetCommand(Amju::OnMultiButton);
  GetElementByName(m_gui, "nogame-button")->SetCommand(Amju::OnNoGameButton);
  GetElementByName(m_gui, "local-button")->SetCommand(Amju::OnLocalButton);
  GetElementByName(m_gui, "edit-button")->SetCommand(Amju::OnEditButton);

  // TODO remember last choice and keep highlighted
  GetElementByName(m_gui, "single-button")->SetHasFocus(true); 
}

bool GSStartMenu::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSStartMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
