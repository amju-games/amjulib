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

static void OnShowSentMsgs()
{
  TheGSVe3Guestbook::Instance()->OnShowSentMsgs();
}

GSVe3Guestbook::GSVe3Guestbook()
{
  m_showLurk = true;
  m_guestbookOnly = true;
  m_showSentMsgs = false;
}

void GSVe3Guestbook::OnShowSentMsgs()
{
std::cout << "Flipping sent/inbox\n";

  m_showSentMsgs = !m_showSentMsgs;
  InitGB();
}

void GSVe3Guestbook::SetIsGuestbookOnly(bool guestbookOnly)
{
  m_guestbookOnly = guestbookOnly;
}

struct GBMsgsForPlayer
{
  GBMsgsForPlayer(int playerId, bool gbOnly, bool showSentMsgs) :
    m_playerId(playerId), m_gbOnly(gbOnly), m_showSent(showSentMsgs) {}

  bool operator()(const MsgManager::Msg& msg)
  {
    bool b = false;
    if (m_showSent)
    {
      b = msg.m_senderId == m_playerId;
    }
    else
    {
      b = msg.m_recipId == m_playerId;
    }

    bool isGBMsg = msg.IsGuestbookMsg();
    if (m_gbOnly)
    {
      return b && isGBMsg;
    }
    return b && !isGBMsg;
  }

  int m_playerId;
  bool m_gbOnly;
  bool m_showSent;
};

void GSVe3Guestbook::InitGB()
{
  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  
  Assert(m_player);
  bool isLocalPlayer = m_player->IsLocalPlayer();

  MsgManager::Msgs msgs = TheMsgManager::Instance()->GetMsgs(
    GBMsgsForPlayer(m_player->GetId(), m_guestbookOnly, m_showSentMsgs));

  // DO show reply buttons IF this is the local player's guestbook/messages
  // (But you can't reply to guestbook msgs, you have to go to the other
  //  player's guestbook and put a comment there.)
  gw->GetGBDisplay()->Init(msgs, isLocalPlayer && !m_guestbookOnly, m_showSentMsgs); 

  // Set title - "Your guestbook" or "<player>'s guestbook"
  // Colour scheme as in other gui states
  GuiText* name = (GuiText*)GetElementByName(m_gui, "title-text");
  Assert(name);
  std::string namestr;
  if (isLocalPlayer)
  {
    namestr = "Your message inbox";
    if (m_showSentMsgs)
    {
      namestr = "Messages sent by you";
      Assert(!m_guestbookOnly); // can't view sent gb msgs
    }

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

    // TODO colours?? Logged in flag is not accurate
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

  // Button to toggle betwen sent and recvd messages.
  GuiButton* sentMsgsButton = (GuiButton*)m_gui->GetElementByName("sent-msgs-button");
  if (sentMsgsButton)
  {
    sentMsgsButton->SetVisible(true);
    sentMsgsButton->SetCommand(Amju::OnShowSentMsgs);
    if (m_showSentMsgs)
    {
      sentMsgsButton->SetText("Back to Inbox");
    }
    else
    {
      sentMsgsButton->SetText("Show sent messages");
    }
  
    if (m_guestbookOnly)
    {
      sentMsgsButton->SetVisible(false);
    }
  }
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

  SetHomeButton();
}

} // namespace
