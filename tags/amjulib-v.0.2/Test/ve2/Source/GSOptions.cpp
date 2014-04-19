#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <SoundManager.h>
#include <ConfigFile.h>
#include "GSOptions.h"
#include "GS2dAvatarMod.h"
#include "GSProxy.h"
#include "Kb.h"
#include "SaveConfig.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnBack()
{
  TheGSOptions::Instance()->SaveToConfig();

  TheGSOptions::Instance()->GoBack();
}

/*
static void OnSound()
{
}

static void OnFullscreen()
{
}
*/

static void OnInternet()
{
  TheGSProxy::Instance()->SetPrevState(TheGSOptions::Instance());
  TheGame::Instance()->SetCurrentState(TheGSProxy::Instance()); 
  // TODO Other internet options, with proxy settings as one page.
}

static void OnKeyboard()
{
  TheGSOptions::Instance()->OnKeyboard();
}

static void OnAvatar()
{
  TheGS2dAvatarMod::Instance()->SetPrevState(TheGSOptions::Instance());
  TheGame::Instance()->SetCurrentState(TheGS2dAvatarMod::Instance()); 
}

GSOptions::GSOptions()
{
  // Default values before set by player
  m_musicVol = 1.0f;
  m_soundVol = 1.0f;
}

void GSOptions::OnKeyboard()
{
  static Kb* kb = TheKb::Instance();
  kb->SetEnabled(!kb->IsEnabled());

  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "keyboard-button");
  if (kb->IsEnabled())
  {
    b->SetText("Disable KB");
  }
  else
  {
    b->SetText("Enable KB");
  }
}

void GSOptions::Update()
{
  GSGui::Update();

}

void GSOptions::Draw()
{
  GSGui::Draw();

}

void GSOptions::Draw2d()
{
  GSGui::Draw2d();
}

void GSOptions::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-options.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
//  GetElementByName(m_gui, "sound-button")->SetCommand(OnSound);
//  GetElementByName(m_gui, "fullscreen-button")->SetCommand(OnFullscreen);
  GetElementByName(m_gui, "inet-button")->SetCommand(OnInternet);
  

  GuiButton* avatar = (GuiButton*)GetElementByName(m_gui, "avatar-button");
  Assert(avatar);
  avatar->SetCommand(OnAvatar);
  bool okToChangeAvatar = (GetLocalPlayer() && GetLocalPlayer()->IsLoggedIn());
  avatar->SetIsEnabled(okToChangeAvatar);  

  LoadFromConfig();

  // TODO Hide for iPad etc, this must always have KB.
  GuiButton* kb = (GuiButton*)GetElementByName(m_gui, "keyboard-button");
  kb->SetCommand(Amju::OnKeyboard);
  if (TheKb::Instance()->IsEnabled())
  {
    kb->SetText("Disable KB");
  }
  else
  {
    kb->SetText("Enable KB");
  }
}

void GSOptions::OnDeactive()
{
  // Save settings to the current PlayerInfo if there is one.
  PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
  if (pi)
  {
    SaveSettingsToPI(pi);
  }

  GSGui::OnDeactive();
}

static const char* MUSIC_VOL_KEY = "options-music-vol";
static const char* KB_KEY = "options-kb";

void GSOptions::LoadFromConfig()
{
  static GameConfigFile* config = TheGameConfigFile::Instance();

  m_musicVol = config->GetFloat(MUSIC_VOL_KEY, m_musicVol);
  bool kb = config->GetInt(KB_KEY, 0) != 0;
  TheKb::Instance()->SetEnabled(kb);
}

void GSOptions::SaveToConfig()
{
  static GameConfigFile* config = TheGameConfigFile::Instance();

  config->SetFloat(MUSIC_VOL_KEY, m_musicVol);
  config->SetInt(KB_KEY, TheKb::Instance()->IsEnabled() ? 1 : 0);

  SaveConfig();
}

void GSOptions::LoadSettingsFromPI(PlayerInfo* pi)
{
  Assert(pi);
  if (pi->Exists(MUSIC_VOL_KEY))
  {
    m_musicVol = pi->PIGetFloat(MUSIC_VOL_KEY);
  }
  TheSoundManager::Instance()->SetSongMaxVolume(m_musicVol);

  if (pi->Exists(KB_KEY))
  {
    bool kb = pi->PIGetBool(KB_KEY);
    TheKb::Instance()->SetEnabled(kb);
  }

  // etc....
}

void GSOptions::SaveSettingsToPI(PlayerInfo* pi)
{
  Assert(pi);
  pi->PISetFloat(MUSIC_VOL_KEY, m_musicVol);
  pi->PISetBool(KB_KEY, TheKb::Instance()->IsEnabled());
}

} // namespace
