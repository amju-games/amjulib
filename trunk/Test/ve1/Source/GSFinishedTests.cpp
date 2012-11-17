#include <AmjuGL.h>
#include <GuiButton.h>
#include "GSFinishedTests.h"
#include "GSTitle.h"

namespace Amju
{
static void OnFinishedTestsOk()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
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
}

} // namespace
