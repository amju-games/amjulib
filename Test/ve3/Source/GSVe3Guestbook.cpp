#include <GuiFactory.h>
#include "GSVe3Guestbook.h"
#include "GuestbookWindow.h"
#include "MsgManager.h"
#include "LocalPlayer.h"

namespace Amju
{
static void OnAddComment()
{
  TheGSVe3Guestbook::Instance()->OnAddCommentButton();
}

static void OnBack()
{
  TheGSVe3Guestbook::Instance()->GoBack();
}

GSVe3Guestbook::GSVe3Guestbook()
{
}

struct ForPlayer
{
  ForPlayer(int playerId) : m_playerId(playerId) {}

  bool operator()(const MsgManager::Msg& msg)
  {
    return true; //msg.m_recipId == m_playerId;
  }

  int m_playerId;
};

void GSVe3Guestbook::InitGB()
{
  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  
  MsgManager::Msgs msgs = TheMsgManager::Instance()->GetMsgs(ForPlayer(m_player->GetId()));
  gw->GetGBDisplay()->Init(msgs); 
}

void GSVe3Guestbook::Update()
{
  GSGui::Update();

  // Check for new msgs
  static MsgManager* mm = TheMsgManager::Instance();
  if (mm->HasNewMsgs())
  {
    mm->ResetNewMsgFlag();
    InitGB();
  }
}

void GSVe3Guestbook::Draw()
{
  GSGui::Draw();

}

void GSVe3Guestbook::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3Guestbook::SetPlayer(Player* player)
{
  m_player = player;
}

void GSVe3Guestbook::OnAddCommentButton()
{
  GuiText* comment = (GuiText*)GetElementByName(m_gui, "guest-comment");
  Assert(comment);
  std::string str = comment->GetText();
  if (str.empty())
  {
    return; // TODO contemptuous sound fx
  }

  int senderId = GetLocalPlayerId();
  int recipId = m_player->GetId();
  Assert(senderId != recipId); // how could this happen?!
  TheMsgManager::Instance()->SendMsg(senderId, recipId, str);
  
}

void GSVe3Guestbook::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui("gui-ve3-guestbook.txt");
  Assert(m_gui);

  // Set title - "Your guestbook" or "<player>'s guestbook"
  // Colour scheme as in other gui states
  GuiText* name = (GuiText*)GetElementByName(m_gui, "title-text");
  Assert(name);
  std::string namestr;

  if (m_player->IsLocalPlayer())
  {
    namestr = "Your guestbook";
    name->SetFgCol(Colour(0, 0.5f, 0, 1));
  }
  else 
  {
    namestr = m_player->GetName() + "'s guestbook";
    if (m_player->IsLoggedIn())
    {
      name->SetFgCol(Colour(1, 0, 0, 1));
    }
    else
    {
      name->SetFgCol(Colour(0, 0, 0, 1));
    }
  }
  name->SetText(namestr);

  InitGB();

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);

  GetElementByName(m_gui, "add-comment-button")->SetCommand(OnAddComment);
}

} // namespace
