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

  m_gui->GetElementByName("single-button")->SetCommand(Amju::OnSingleButton);
  m_gui->GetElementByName("multi-button")->SetCommand(Amju::OnMultiButton);
  m_gui->GetElementByName("nogame-button")->SetCommand(Amju::OnNoGameButton);
  m_gui->GetElementByName("local-button")->SetCommand(Amju::OnLocalButton);
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
