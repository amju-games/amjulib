#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <SoundManager.h>
#include "GSOptions.h"
#include "GSAvatarMod.h"
#include "GSProxy.h"
#include "Kb.h"

namespace Amju
{
static void OnBack()
{
  TheGSOptions::Instance()->GoBack();
}

static void OnSound()
{
}

static void OnFullscreen()
{
}

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
  GetElementByName(m_gui, "sound-button")->SetCommand(OnSound);
  GetElementByName(m_gui, "fullscreen-button")->SetCommand(OnFullscreen);
  GetElementByName(m_gui, "inet-button")->SetCommand(OnInternet);

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
