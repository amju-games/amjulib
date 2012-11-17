#include <AmjuGL.h>
#include <GuiButton.h>
#include "GSLogout.h"
#include "LogOut.h"

namespace Amju
{
static void OnLogoutCancel()
{
  TheGSLogout::Instance()->GoBack();
}

GSLogout::GSLogout()
{
}

void GSLogout::Update()
{
  GSGui::Update();
}

void GSLogout::Draw()
{
  GSGui::Draw();

}

void GSLogout::Draw2d()
{
  GSGui::Draw2d();
  SendLogOut(); // blocks
  GoBack();
}

void GSLogout::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-logout.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(OnLogoutCancel);
}

} // namespace
