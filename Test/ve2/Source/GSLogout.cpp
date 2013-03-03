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

// Count frames displayed since state activated, so we flip at least once
//  before blocking
static int count = 0;

void GSLogout::Update()
{
  GSGui::Update();
  count++;
}

void GSLogout::Draw()
{
  GSGui::Draw();
}

void GSLogout::Draw2d()
{
  GSGui::Draw2d();
  if (count == 2) // make sure we flip before blocking on sending request
  {
    SendLogOut(); // blocks
    GoBack();
  }
}

void GSLogout::OnActive()
{
  count = 0;
  GSGui::OnActive();

  m_gui = LoadGui("gui-logout.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(OnLogoutCancel);
}

} // namespace
