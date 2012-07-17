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
#include "AvatarManager.h"
#include "Version.h"
#include "SpecialConfig.h"
#include "PlayerInfo.h"
#include "LurkMsg.h" // TODO TEMP TEST

namespace Amju
{
static void OnQuickStartButton()
{
  static PlayerInfoManager* pim = ThePlayerInfoManager::Instance();
  PlayerInfo* pi = pim->GetPI();
  Assert(pi);
  std::string email = pi->PIGetString(PI_KEY("email"));
  TheGSLoginWaiting::Instance()->SetEmail(email);
  TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
}

static void OnStartButton()
{
  // TODO TEMP TEST
  TheLurker::Instance()->Queue(LurkMsg("hello", Colour(1, 1, 1, 1), Colour(0, 0, 0, 1), AMJU_TOP));
  return; 


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

  TheLurker::Instance()->Update();
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

  // TODO TEMP TEST
  TheLurker::Instance()->Draw();
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

  GuiText* ver = (GuiText*)GetElementByName(m_gui, "version");
  std::string s = "v." + ToString(VersionMajor) + "." + ToString(VersionMinor);
#ifdef _DEBUG
  s += " DEBUG";
#endif
  ver->SetText(s);

  CreateText("my game");

  // TODO CONFIG
  TheSoundManager::Instance()->PlaySong("/Sound/apz1.it");
}

void GSTitle::OnDeactive()
{
  GSGui::OnDeactive();
  CreateText("");
}

} // namespace
