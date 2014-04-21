#include <GuiFactory.h>
#include <GuiButton.h>
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
  m_guestbookOnly = true;
  m_showLurk = true;
}

void GSVe3Guestbook::SetIsGuestbookOnly(bool guestbookOnly)
{
  m_guestbookOnly = guestbookOnly;
}

struct GBMsgsForPlayer
{
  GBMsgsForPlayer(int playerId, bool gbOnly) : m_playerId(playerId), m_gbOnly(gbOnly) {}

  bool operator()(const MsgManager::Msg& msg)
  {
    if (m_gbOnly)
    {
      return msg.IsGuestbookMsg() && msg.m_recipId == m_playerId;
    }
    return msg.m_recipId == m_playerId;
  }

  int m_playerId;
  bool m_gbOnly;
};

void GSVe3Guestbook::InitGB()
{
  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  
  Assert(m_player);
  bool isLocalPlayer = m_player->IsLocalPlayer();

  MsgManager::Msgs msgs = TheMsgManager::Instance()->GetMsgs(
    GBMsgsForPlayer(m_player->GetId(), m_guestbookOnly));

  // DO show reply buttons IF this is the local player's guestbook/messages
  // (But you can't reply to guestbook msgs, you have to go to the other
  //  player's guestbook and put a comment there.)
  gw->GetGBDisplay()->Init(msgs, isLocalPlayer && !m_guestbookOnly); 
}

void GSVe3Guestbook::Update()
{
  GSGui::Update();

  // Check for new msgs
  static MsgManager* mm = TheMsgManager::Instance();
  if (mm->HasNewMsgs())
  {
    mm->ResetNewMsgs();
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
  // Add GB tag so we can only list GB msgs if we want
  str += "\n<gb>";

  int senderId = GetLocalPlayerId();
  int recipId = m_player->GetId();
  Assert(senderId != recipId); // how could this happen?!
  TheMsgManager::Instance()->SendMsg(senderId, recipId, str);
  comment->SetText("");
}

void GSVe3Guestbook::OnDeactive()
{
  GuiText* comment = (GuiText*)m_gui->GetElementByName("guest-comment"); // can fail
  if (comment)
  {
    m_unfinishedCommentStr = comment->GetText();
  }

  GSGui::OnDeactive(); // Duh, do this last as it destroys m_gui
}

void GSVe3Guestbook::OnActive()
{
  GSGui::OnActive();

  Assert(m_player);
  bool isLocalPlayer = m_player->IsLocalPlayer();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui(isLocalPlayer ? "gui-ve3-my-guestbook.txt" : "gui-ve3-guestbook.txt");
  Assert(m_gui);

  // Set title - "Your guestbook" or "<player>'s guestbook"
  // Colour scheme as in other gui states
  GuiText* name = (GuiText*)GetElementByName(m_gui, "title-text");
  Assert(name);
  std::string namestr;

  if (isLocalPlayer)
  {
    namestr = "Your messages";
    if (m_guestbookOnly)
    {
      namestr = "Your guestbook";
    }
    name->SetFgCol(Colour(0, 0.5f, 0, 1));
  }
  else 
  {
    // Maybe get rid of this to simplify the game

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

  GuiButton* addcomment = (GuiButton*)m_gui->GetElementByName("add-comment-button");
  if (addcomment)
  {
    addcomment->SetCommand(OnAddComment);
  }

  GuiText* comment = (GuiText*)m_gui->GetElementByName("guest-comment"); // can fail
  if (comment)
  {
    comment->SetHasFocus(true);
    comment->SetText(m_unfinishedCommentStr);
  }
}

} // namespace
