#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
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
} // namespace
