#include <GuiFactory.h>
#include "GSVe3Guestbook.h"
#include "GuestbookWindow.h"

namespace Amju
{
static GuiElement* CreateGuestbook()
{
  return new GuestbookWindow;
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
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbook);
  m_gui = LoadGui("gui-ve3-guestbook.txt");
  Assert(m_gui);



  // TODO Set focus element, cancel element, command handlers
}

} // namespace
