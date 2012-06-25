#include <GuiText.h>
#include <GuiTextEdit.h>
#include <GuiWindow.h>
#include <GuiButton.h>
#include "ChatConsole.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"

namespace Amju
{
static void OnChatSendButton()
{
  TheChatConsole::Instance()->OnChatSend();
}

static void OnChatCancelButton()
{
  TheChatConsole::Instance()->OnChatCancel();
}

//static void OnRecvCloseButton()
//{
//  TheChatConsole::Instance()->OnRecvClose();
//}

void ChatConsole::Conversation::Draw()
{
  PushColour();

  float pos = -0.6f; // TODO CONFIG
  for (int i = m_texts.size() - 1; i >= 0; i--)
  {
    GuiText* text = m_texts[i];
    pos += text->GetSize().y;
    text->SetLocalPos(Vec2f(-0.8f, pos));
    text->Draw();

    MultColour(Colour(1, 1, 1, 0.6f)); // fade
  }

  PopColour();
}

void ChatConsole::Conversation::AddText(bool sentNotRecv, const std::string& msg)
{
  GuiText* text = new GuiText;
  text->SetIsMulti(true); // So we can see all of a long msg!
  text->SetSize(Vec2f(1.6f, 0.1f)); //text->GetSize().y)); // TODO multi line msgs
  text->SetTextSize(1.0f);
  text->SetText(msg);
  //text->SizeToText(); 
  text->SetSize(Vec2f(1.6f, (float)text->GetNumLines() * 0.1f)); // TODO multi line msgs
  text->SetDrawBg(true);
  text->SetInverse(sentNotRecv);
  text->SetJust(GuiText::AMJU_JUST_LEFT);
  if (sentNotRecv)
  {
    //text->SetJust(GuiText::AMJU_JUST_RIGHT);
    //text->SetFgCol(Colour(0, 0, 0, 1));
  }

  m_texts.push_back(text);
}

ChatConsole::ChatConsole()
{
  m_lastRecipId = -1;
  m_chatSendIsActive = false;
  m_chatRecvIsActive = false;
}

void ChatConsole::Draw()
{
  if (m_gui)
  {
    m_gui->Draw();

    if (m_lastRecipId != -1)
    {
      m_convs[m_lastRecipId].Draw();
    }
  }
}

bool ChatConsole::IsActive() const
{
  return (m_gui != 0 && m_chatSendIsActive);
}

void ChatConsole::OnDeactive()
{
  m_gui = 0;
}

void ChatConsole::SetPlayerIsTyping(bool isTyping, int typerId, int recipId)
{
  // TODO
  if (isTyping)
  {
std::cout << "This player: " << typerId << " is typing a message for " << recipId << ".\n";
    if (recipId == m_lastRecipId)
    {
std::cout << "Holy shit, that's ME!!\n";
    }
  }
  else
  {
std::cout << "This player: " << typerId << " is not typing anything.\n";
  }
}

void OnTyping()
{
std::cout << "Typing!!\n";
  TheChatConsole::Instance()->OnTyping();
}

void ChatConsole::OnTyping()
{
  // Set state for the local player so the other client can see that we are typing something.
  int recip = m_lastRecipId;
  GuiTextEdit* textedit = (GuiTextEdit*)GetElementByName(m_gui, "chat-text-edit");
  if (textedit->GetText().empty())
  {
    // No text
    recip = 0;
  }

  int senderId = GetLocalPlayer()->GetId();
  TheObjectUpdater::Instance()->SendUpdateReq(senderId, SET_KEY("istyping"), ToString(recip));
}

void ChatConsole::OnActive()
{
  m_gui = LoadGui("gui-chat.txt");
  Assert(m_gui);

  GuiButton* send = (GuiButton*)GetElementByName(m_gui, "chat-send-button");
  send->SetCommand(Amju::OnChatSendButton);
  //send->SetIsFocusButton(true);
  send->SetHasFocus(true); // use in preference to SetIsFocusButton

  GuiTextEdit* textedit = (GuiTextEdit*)GetElementByName(m_gui, "chat-text-edit");
  textedit->SetText("");
  textedit->SetOnCharFunc(Amju::OnTyping);

  GetElementByName(m_gui, "chat-cancel-button")->SetCommand(Amju::OnChatCancelButton);
  ////GetElementByName(m_gui, "recv-close-button")->SetCommand(Amju::OnRecvCloseButton);
  ActivateChatSend(false, -1);
  ActivateChatRecv(false);
}

void ChatConsole::OnChatSend()
{
  Assert(m_lastRecipId != -1);
  Assert(GetLocalPlayer()); // or couldn't send, right ?

  int senderId = GetLocalPlayer()->GetId();
  GuiTextEdit* textedit = (GuiTextEdit*)GetElementByName(m_gui, "chat-text-edit");
  std::string text = textedit->GetText();

  if (text == "")
  {
    // TODO Fail sound effect etc
std::cout << "Blank msg, not sending.\n";
    return;
  }

  TheMsgManager::Instance()->SendMsg(senderId, m_lastRecipId, text);

  m_convs[m_lastRecipId].AddText(true, text);

  //ActivateChatSend(false, -1); // ??
  textedit->SetText("");

  // Reset typing flag
  OnTyping();

  GetElementByName(m_gui, "msg-recv-comp")->SetVisible(true);
}

void ChatConsole::OnChatCancel()
{
  ActivateChatSend(false, -1);
}

void ChatConsole::OnRecvClose()
{
  ActivateChatRecv(false);
}

bool ChatConsole::CanShowMsg() const
{
  return true; // TODO !m_chatRecvIsActive; // && !m_chatSendIsActive ??
}

void ChatConsole::ActivateChatSend(bool active, int recipId)
{
  // TODO polish -- jump onto screen
  m_lastRecipId = recipId;

  if (active)
  {
std::cout << "Activate chat -- recip ID = " << recipId << "\n";

    std::string recipName;
    GetNameForPlayer(recipId, &recipName);

    m_gui->GetElementByName("chat-comp")->SetVisible(true);
    ((GuiText*)m_gui->GetElementByName("chat-recip-name"))->SetText(recipName);
    GuiTextEdit* textedit = (GuiTextEdit*)GetElementByName(m_gui, "chat-text-edit");
    textedit->SetHasFocus(true);
    //textedit->SetText("");
  }
  else
  {
    m_gui->GetElementByName("chat-comp")->SetVisible(false);
  }
  m_chatSendIsActive = active;
}

void ChatConsole::ActivateChatRecv(bool active, const MsgManager::Msg* msg)
{
  if (active && msg)
  {
    std::string senderName;
    if (!GetNameForPlayer(msg->m_senderId, &senderName))
    {
      return;
    }

    //((GuiText*)m_gui->GetElementByName("msg-recv-sender"))->SetText(senderName);
    //((GuiText*)m_gui->GetElementByName("msg-recv-text"))->SetText(msg->m_text);
    
    // TODO TEMP TEST
    m_lastRecipId = msg->m_senderId;

    m_convs[m_lastRecipId].AddText(false, msg->m_text);

  }

  GetElementByName(m_gui, "msg-recv-comp")->SetVisible(active);
  m_chatRecvIsActive = active;
}

}

