#include "GSCogTestMenu.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSStartMenu.h"
#include "GSLetterCancellation1.h"

namespace Amju
{
static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
}

static void OnLetterSigmaButton()
{
  TheGSLetterCancellation1::Instance()->LoadConfig("letter-cancel-sigma.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation1::Instance());
}

static void OnLetterMButton()
{
  TheGSLetterCancellation1::Instance()->LoadConfig("letter-cancel-m.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation1::Instance());
}

GSCogTestMenu::GSCogTestMenu()
{
}

void GSCogTestMenu::Update()
{
  GSGui::Update();

}

void GSCogTestMenu::Draw()
{
  GSGui::Draw();

}

void GSCogTestMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestMenu::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-cogtestmenu.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "cancel-button")->SetCommand(Amju::OnCancelButton);
  GetElementByName(m_gui, "letter-m-button")->SetCommand(Amju::OnLetterMButton);
  GetElementByName(m_gui, "letter-sigma-button")->SetCommand(Amju::OnLetterSigmaButton);

}

} // namespace
