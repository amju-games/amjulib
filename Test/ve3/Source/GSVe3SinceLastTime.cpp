#include <GuiFactory.h>
#include "GSVe3SinceLastTime.h"
#include "GuestbookWindow.h"
#include "MsgManager.h"

namespace Amju
{
static void OnBack()
{
  TheGSVe3SinceLastTime::Instance()->GoBack();
}

GSVe3SinceLastTime::GSVe3SinceLastTime()
{
}

void GSVe3SinceLastTime::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui("gui-ve3-since-last-time.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);

  InitGB();
}

} // namespace
