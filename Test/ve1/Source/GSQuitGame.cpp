#include "GSQuitGame.h"
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSTitle.h"

namespace Amju
{
static void OnQuitSession()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

static void OnQuitProcess()
{
  exit(0);
}

static void OnCancel()
{
  TheGSQuitGame::Instance()->GoBack();
}

GSQuitGame::GSQuitGame()
{
}

void GSQuitGame::Update()
{
  GSGui::Update();
}

void GSQuitGame::Draw()
{
  GSGui::Draw();
}

void GSQuitGame::Draw2d()
{
  GSGui::Draw2d();
}

void GSQuitGame::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-quitgame.txt");
  Assert(m_gui);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancel);
  cancel->SetHasFocus(true); 

  GuiButton* qsession = (GuiButton*)GetElementByName(m_gui, "quitsession-button");
  qsession->SetCommand(Amju::OnQuitSession);

  GuiButton* qprocess = (GuiButton*)GetElementByName(m_gui, "quitprocess-button");
  qprocess->SetCommand(Amju::OnQuitProcess);

}

} // namespace
