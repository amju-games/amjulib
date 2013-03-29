#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include "GSCogResults.h"
#include "CogTestNag.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnCogTestResultsOk()
{
  // TODO does this work from anywhere ??
  static GSCogResults* cr = TheGSCogResults::Instance();
  if (cr->GetPrevState())
  {
    cr->GoBack();
  }
  else
  {
    OnCogTestsAllDone();
  }
}

GSCogResults::GSCogResults()
{
}

void GSCogResults::Update()
{
  GSGui::Update();

}

void GSCogResults::Draw()
{
  GSGui::Draw();

}

void GSCogResults::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogResults::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-cogresults.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnCogTestResultsOk);
}

} // namespace
