#include <AmjuFirst.h>
#include "GSAdminMenu.h"
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSStartMenu.h"
#include "GameMode.h"
#include "GSChoosePlayer.h"
#include "GSCogTestMenu.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnCancelButton(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
}

static void OnEditButton(GuiElement*)
{
  SetGameMode(AMJU_MODE_EDIT);
  TheGame::Instance()->SetCurrentState(TheGSChoosePlayer::Instance());
}

static void OnCogTestButton(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

GSAdminMenu::GSAdminMenu()
{
}

void GSAdminMenu::Update()
{
  GSGui::Update();

}

void GSAdminMenu::Draw()
{
  GSGui::Draw();

}

void GSAdminMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSAdminMenu::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-adminmenu.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
//  GetElementByName(m_gui, "local-button")->SetCommand(Amju::OnLocalButton);
  GetElementByName(m_gui, "edit-button")->SetCommand(Amju::OnEditButton);
  GetElementByName(m_gui, "cog-button")->SetCommand(Amju::OnCogTestButton);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);
}

} // namespace
