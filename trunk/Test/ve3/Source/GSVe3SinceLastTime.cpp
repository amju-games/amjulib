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

void GSVe3SinceLastTime::Update()
{
  GSGui::Update();

  // Check for new msgs
  static MsgManager* mm = TheMsgManager::Instance();
  if (mm->HasNewMsgs())
  {
    mm->ResetNewMsgFlag();

    GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
    Assert(gw);
    gw->GetGBDisplay()->Init(m_player); 
  }
}

void GSVe3SinceLastTime::Draw()
{
  GSGui::Draw();
}

void GSVe3SinceLastTime::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3SinceLastTime::SetPlayer(Player* player)
{
  m_player = player;
}

void GSVe3SinceLastTime::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui("gui-ve3-since-last-time.txt");
  Assert(m_gui);

  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  gw->GetGBDisplay()->Init(m_player); 

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
}

} // namespace
