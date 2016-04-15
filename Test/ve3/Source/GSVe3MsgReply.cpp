#include <GuiTextEdit.h>
#include "GSVe3MsgReply.h"
#include "MsgManager.h"
#include "MugShot.h"
#include "Player.h"
#include "ObjectManager.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "ShowMsgHelper.h"

namespace Amju
{
static void OnReplyCancel(GuiElement*)
{
  TheGSVe3MsgReply::Instance()->GoBack();
}

static void OnReplySend(GuiElement*)
{
  TheGSVe3MsgReply::Instance()->SendReply();
}

GSVe3MsgReply::GSVe3MsgReply()
{
  m_showLurk = true;
  m_msgId = -1;
}

void GSVe3MsgReply::Update()
{
  GSGui::Update();

}

void GSVe3MsgReply::Draw()
{
  GSGui::Draw();

}

void GSVe3MsgReply::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3MsgReply::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-msg-reply.txt");
  Assert(m_gui);

  // Get msg from msg manager.
  Assert(m_msgId != -1);
  ShowMsgHelper(m_msgId, m_gui);

/* -- now in helper

  const MsgManager::Msg* msg = TheMsgManager::Instance()->GetMsg(m_msgId);
  Assert(msg);

  // Display msg text
  GuiText* text = (GuiText*)GetElementByName(m_gui, "orig-msg-text");
  Assert(text);
  text->SetText(msg->m_text);

  // mugshot of sender
  Player* sender = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  if (sender)
  {
    // TODO display this under mugshot
    std::string senderName = sender->GetName();

    Mugshot* mug = new Mugshot;
    mug->Init(sender);
    GuiComposite* comp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
    if (comp)
    {
      mug->SetLocalPos(Vec2f(-0.9f, 0.7f));
      comp->AddChild(mug);
    }
  }
*/

  // Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(Amju::OnReplyCancel);
  GetElementByName(m_gui, "send-button")->SetCommand(Amju::OnReplySend);

  SetHomeButton();
}

void GSVe3MsgReply::SendReply()
{
  // Get msg text, sanity check it
  GuiTextEdit* edit = (GuiTextEdit*)GetElementByName(m_gui, "reply-text");
  Assert(edit);
  std::string text = edit->GetText();

  // Get original Msg
  MsgManager* mm = TheMsgManager::Instance();
  Assert(m_msgId != -1);
  const MsgManager::Msg* msg = mm->GetMsg(m_msgId);
  Assert(msg);
  
  // Make new msg, reversing sender & recip, with reply text
  int newSender = msg->m_recipId;
  int newRecip = msg->m_senderId;
  mm->SendMsg(newSender, newRecip, text);

  // Lurk msg then go back
  std::string str = "OK, your reply was sent!";
  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnReplyCancel); 
  TheLurker::Instance()->Queue(lm);    
}
} // namespace
