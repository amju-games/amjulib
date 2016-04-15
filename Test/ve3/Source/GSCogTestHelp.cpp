#include "GSCogTestHelp.h"
#include <AmjuGL.h>

namespace Amju
{
static void OnHelpDone(GuiElement*)
{
  TheGSCogTestHelp::Instance()->GoBack();
}

GSCogTestHelp::GSCogTestHelp()
{
}

void GSCogTestHelp::Update()
{
  GSGui::Update();

}

void GSCogTestHelp::Draw()
{
  GSGui::Draw();

}

void GSCogTestHelp::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestHelp::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-cogtest-help.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "done-button")->SetCommand(OnHelpDone);
}

} // namespace
