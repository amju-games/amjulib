#include <GuiFactory.h>
#include "GSVe3Guestbook.h"
#include "GuestbookWindow.h"
#include "LocalPlayer.h"

namespace Amju
{
static void OnBack()
{
  TheGSVe3Guestbook::Instance()->GoBack();
}

GSVe3Guestbook::GSVe3Guestbook()
{
}

void GSVe3Guestbook::Update()
{
  GSGui::Update();

}

void GSVe3Guestbook::Draw()
{
  GSGui::Draw();

}

void GSVe3Guestbook::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3Guestbook::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui("gui-ve3-guestbook.txt");
  Assert(m_gui);

  // TODO Set title - "Your guestbook" or "<player>'s guestbook"
  // Colour scheme as in other gui states

  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  Player* player = GetLocalPlayer(); // TODO current player for this page 
  gw->GetGBDisplay()->Init(player); 

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
}

} // namespace
