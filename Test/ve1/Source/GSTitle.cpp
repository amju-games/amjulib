#include "GSTitle.h"
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <CursorManager.h>
#include "GSStartMenu.h"
#include "GSLogin.h"
#include "GSChoosePlayer.h"
#include "GSYesNoQuitProcess.h"
#include "AvatarManager.h"
#include "Version.h"

namespace Amju
{
static void OnStartButton()
{
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());

  // For now, no start menu as there is only the multi player option
  //TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnQuitButton()
{
  // No Confirm from Title screen
  exit(0);
 
  //TheGSYesNoQuitProcess::Instance()->SetPrevState(TheGSTitle::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSYesNoQuitProcess::Instance());
}

GSTitle::GSTitle()
{
}

void GSTitle::Update()
{
  GSGui::Update();

}

void GSTitle::Draw()
{
  GSGui::Draw();
}

void GSTitle::Draw2d()
{
  GSGui::Draw2d();
}

void GSTitle::OnActive()
{
  static bool first = true;
  if (first)
  {
    first = false;

    TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f));
    TheAvatarManager::Instance()->Load();
  }

  GSGui::OnActive();

  m_gui = LoadGui("gui-title.txt");
  Assert(m_gui);

  GuiButton* start = (GuiButton*)GetElementByName(m_gui, "start-button");
  start->SetCommand(Amju::OnStartButton);
  start->SetHasFocus(true); // use in preference to SetIsFocusButton

  GuiButton* quit = (GuiButton*)GetElementByName(m_gui, "quit-button");
  quit->SetCommand(Amju::OnQuitButton);
  quit->SetIsCancelButton(true);

  GuiText* ver = (GuiText*)GetElementByName(m_gui, "version");
  std::string s = "v." + ToString(VersionMajor) + "." + ToString(VersionMinor);
#ifdef _DEBUG
  s += " DEBUG";
#endif
  ver->SetText(s);

  CreateText("my game");
}

void GSTitle::OnDeactive()
{
  GSGui::OnDeactive();
  CreateText("");
}

} // namespace
