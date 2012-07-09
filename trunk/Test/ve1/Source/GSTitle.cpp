#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <CursorManager.h>
#include <SoundManager.h>
#include "GSTitle.h"
#include "GSStartMenu.h"
#include "GSLogin.h"
#include "GSChoosePlayer.h"
#include "GSYesNoQuitProcess.h"
#include "AvatarManager.h"
#include "Version.h"
#include "SpecialConfig.h"
#include "GSFade.h"
#include "PlayerInfo.h"

namespace Amju
{
static void OnStartButton()
{
  // If no players, go straight to log in screen
  if (ThePlayerInfoManager::Instance()->GetNumPlayerNames() == 0)
  {
    TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
  }
  else
  {
    // Menu is back in -- so we can options (and admin menu) before starting
    TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
  }


  // Ideally
  //Fade(TheGSStartMenu::Instance(), 2.0f);
}

static void OnQuitButton()
{
  // No Confirm from Title screen
  exit(0);
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

  // Draw env info, etc.
  static GuiText t;

  t.SetSize(Vec2f(1.0f, 0.1f));
  t.SetJust(GuiText::AMJU_JUST_LEFT);
  t.SetInverse(true);
  t.SetDrawBg(true);

  t.SetLocalPos(Vec2f(-1.0f, 0.8f));
  std::string s = "SaveDir: " + GetAppName();
  t.SetText(s);
  t.Draw();

  t.SetLocalPos(Vec2f(-1.0f, 0.7f));
  s = "Server: " + GetServer();
  t.SetText(s);
  t.Draw();

  t.SetLocalPos(Vec2f(-1.0f, 0.6f));
  s = "Env: " + GetEnv();
  t.SetText(s);
  t.Draw();
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

  TheSoundManager::Instance()->PlaySong("/Sound/apz1.it");
}

void GSTitle::OnDeactive()
{
  GSGui::OnDeactive();
  CreateText("");
}

} // namespace
