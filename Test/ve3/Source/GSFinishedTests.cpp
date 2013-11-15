#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include <Game.h>
#include "GSFinishedTests.h"
#include "GSLogout.h"
#include "GSTitle.h"
#include "GSCogResults.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnResults()
{
  TheGSCogResults::Instance()->SetPrevState(TheGSFinishedTests::Instance());
  TheGame::Instance()->SetCurrentState(TheGSCogResults::Instance());
}

static void OnFinishedTestsOk()
{
  TheGSLogout::Instance()->SetPrevState(TheGSTitle::Instance());
  TheGame::Instance()->SetCurrentState(TheGSLogout::Instance());
}

GSFinishedTests::GSFinishedTests()
{
}

void GSFinishedTests::Update()
{
  GSGui::Update();
}

void GSFinishedTests::Draw()
{
  GSGui::Draw();
}

void GSFinishedTests::Draw2d()
{
  GSGui::Draw2d();
}

void GSFinishedTests::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-finishedtests.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnFinishedTestsOk);

  GuiButton* results = (GuiButton*)GetElementByName(m_gui, "results-button");
  results->SetCommand(OnResults);
}

} // namespace
