#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <CursorManager.h>
#include <SoundManager.h>
#include "GSTitle.h"
#include "GSStartMenu.h"
#include "GSLogin.h"
#include "GSLoginWaiting.h"
#include "GSChoosePlayer.h"
#include "GSYesNoQuitProcess.h"
#include "GSOptions.h"
#include "AvatarManager.h"
#include "Version.h"
#include "SpecialConfig.h"
#include "PlayerInfo.h"
#include "GameMode.h"
#include "LurkMsg.h" // so we can display msgs in title screen
#include "ROConfig.h"
#include "Kb.h"

#define SHOW_ENV_INFO
#define SHOW_FRAME_TIME
#define SHOW_VERSION

namespace Amju
{
static void OnQuickStartButton()
{
  SetGameMode(AMJU_MODE_NONE);
  static PlayerInfoManager* pim = ThePlayerInfoManager::Instance();
  PlayerInfo* pi = pim->GetPI();
  Assert(pi);
  std::string email = pi->PIGetString(PI_KEY("email"));
  TheGSLoginWaiting::Instance()->SetEmail(email);
  TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
}

static void OnOptionsButton()
{
  TheGSOptions::Instance()->SetPrevState(TheGSTitle::Instance());
  TheGame::Instance()->SetCurrentState(TheGSOptions::Instance());
}

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
  m_showLurk = true;
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

#ifdef SHOW_ENV_INFO
  // Draw env info, etc.
  static GuiText t;

  t.SetSize(Vec2f(1.0f, 0.1f));
  t.SetJust(GuiText::AMJU_JUST_LEFT);
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
#endif
}

void GSTitle::OnActive()
{
  static bool first = true;
  if (first)
  {
    first = false;

    TheAvatarManager::Instance()->Load();

    // Set default keyboard layout
    KbSetLayout(KB_LAYOUT_REGULAR);

    TheGSOptions::Instance()->LoadFromConfig();
  }

#ifdef SHOW_FRAME_TIME
  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  TheGame::Instance()->SetFrameTimeFont(font);
#endif

  GSGui::OnActive();

  m_gui = LoadGui("gui-title.txt");
  Assert(m_gui);

  GuiButton* start = (GuiButton*)GetElementByName(m_gui, "start-button");
  start->SetCommand(Amju::OnStartButton);
  start->SetHasFocus(true); 

  GuiButton* options = (GuiButton*)GetElementByName(m_gui, "options-button");
  options->SetCommand(Amju::OnOptionsButton);

  GuiButton* quick = (GuiButton*)GetElementByName(m_gui, "quick-start-button");
  static PlayerInfoManager* pim = ThePlayerInfoManager::Instance();
  if (pim->GetNumPlayerNames() > 0)
  {
    quick->SetVisible(true);
    quick->SetCommand(Amju::OnQuickStartButton);
    quick->SetHasFocus(true);

    // Change button text to player name
    Strings names = pim->GetPlayerNames();
    Assert(!names.empty());
    pim->SetCurrentPlayer(names[0]);
    PlayerInfo* pi = pim->GetPI();
    Assert(pi);
    std::string playername = pi->PIGetString(PI_KEY("playername"));
    quick->SetText(playername);
  }
  else
  {
    quick->SetVisible(false);
  }

  GuiButton* quit = (GuiButton*)GetElementByName(m_gui, "quit-button");
  quit->SetCommand(Amju::OnQuitButton);
  quit->SetIsCancelButton(true);

#ifdef SHOW_VERSION
  GuiText* ver = (GuiText*)GetElementByName(m_gui, "version");
  std::string s = "v." + ToString(VersionMajor) + "." + ToString(VersionMinor);
#ifdef _DEBUG
  s += " DEBUG";
#endif
  ver->SetText(s);
#endif

  CreateText("my game");

#ifdef PLAY_MUSIC
  TheSoundManager::Instance()->PlaySong(ROConfig()->GetValue("music-title", "Sound/hammers.it"));
#endif
}

void GSTitle::OnDeactive()
{
#ifdef PLAY_MUSIC
  TheSoundManager::Instance()->PlaySong(ROConfig()->GetValue("music-menu", "Sound/piano.it"));
#endif

  GSGui::OnDeactive();
  CreateText("");
}

} // namespace
