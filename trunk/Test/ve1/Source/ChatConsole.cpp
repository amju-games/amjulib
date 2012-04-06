#include <GuiText.h>
#include <GuiTextEdit.h>
#include "ChatConsole.h"
#include "LocalPlayer.h"

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

static void OnRecvCloseButton()
{
  TheChatConsole::Instance()->OnRecvClose();
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

void ChatConsole::OnActive()
{
  m_gui = LoadGui("gui-chat.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "chat-send-button")->SetCommand(Amju::OnChatSendButton);
  GetElementByName(m_gui, "chat-cancel-button")->SetCommand(Amju::OnChatCancelButton);
  GetElementByName(m_gui, "recv-close-button")->SetCommand(Amju::OnRecvCloseButton);
  ActivateChatSend(false, -1);
  ActivateChatRecv(false);
}

void ChatConsole::OnChatSend()
{
  Assert(m_lastRecipId != -1);
  Assert(GetLocalPlayer()); // or couldn't send, right ?

  int senderId = GetLocalPlayer()->GetId();
  std::string text =
    ((GuiTextEdit*)m_gui->GetElementByName("chat-text-edit"))->GetText();
  TheMsgManager::Instance()->SendMsg(senderId, m_lastRecipId, text);

  ActivateChatSend(false, -1); // ??
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
  return !m_chatRecvIsActive; // && !m_chatSendIsActive ??
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
    textedit->SetText("");
  }
  else
  {
    m_gui->GetElementByName("chat-comp")->SetVisible(false);
  }
  m_chatSendIsActive = active;
}

void ChatConsole::ActivateChatRecv(bool active, const MsgManager::Msg* msg)
{
  if (active)
  {
    std::string senderName;
    GetNameForPlayer(msg->m_senderId, &senderName);

    ((GuiText*)m_gui->GetElementByName("msg-recv-sender"))->SetText(senderName);
    ((GuiText*)m_gui->GetElementByName("msg-recv-text"))->SetText(msg->m_text);
  }

  m_gui->GetElementByName("msg-recv-comp")->SetVisible(active);
  m_chatRecvIsActive = active;
}

}

