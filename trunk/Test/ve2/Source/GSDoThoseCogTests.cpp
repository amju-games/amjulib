#include <AmjuGL.h>
#include <GuiButton.h>
#include <Game.h>
#include "GSDoThoseCogTests.h"
#include "GSQuitGame.h"

namespace Amju
{
static void OnDoCogTestsOk()
{
  TheGSDoThoseCogTests::Instance()->GoBack();
}

static void OnDoCogTestsQuit()
{
  TheGSQuitGame::Instance()->SetPrevState(TheGSDoThoseCogTests::Instance());
  TheGame::Instance()->SetCurrentState(TheGSQuitGame::Instance());
}

GSDoThoseCogTests::GSDoThoseCogTests()
{
}

void GSDoThoseCogTests::Update()
{
  GSGui::Update();
}

void GSDoThoseCogTests::Draw()
{
  GSGui::Draw();
}

void GSDoThoseCogTests::Draw2d()
{
  GSGui::Draw2d();
}

void GSDoThoseCogTests::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-dothosecogtests.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnDoCogTestsOk);

  GuiButton* quit = (GuiButton*)GetElementByName(m_gui, "quit-button");
  quit->SetCommand(OnDoCogTestsQuit);
}

} // namespace
