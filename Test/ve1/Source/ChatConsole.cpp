#include <GuiText.h>
#include <GuiTextEdit.h>
#include <GuiWindow.h>
#include <GuiButton.h>
#include <GuiRect.h>
#include <EventPoller.h>
#include <Screen.h>
#include <Timer.h>
#include "ChatConsole.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "GSMain.h"
#include "Kb.h"
#include "Player.h"
#include "LurkMsg.h" 

namespace Amju
{
static Colour RECIP_COLOUR(0.5f, 0, 0.5f, 1);

static void OnChatSendButton()
{
  TheChatConsole::Instance()->OnChatSend();
}

static void OnChatCancelButton()
{
  TheChatConsole::Instance()->OnChatCancel();
}

void ChatConsole::Conversation::Draw()
{
  static ChatConsole* cc = TheChatConsole::Instance();

  float pos = cc->m_pos.y + 0.25f; 
  for (int i = m_phrases.size() - 1; i >= 0; i--)
  {
    GuiText* text = m_phrases[i].m_text;
    pos += text->GetSize().y;
    text->SetLocalPos(Vec2f(cc->m_pos.x - 0.025f, pos));

    if (!m_phrases[i].m_sentByMe)
    {
      GuiRect r;
      r.SetLocalPos(text->GetCombinedPos());
      r.SetSize(text->GetSize() + Vec2f(0.01f, 0));
      r.SetCornerRadius(0.02f);
      r.SetColour(RECIP_COLOUR); 

      // Corners: is phrase above also from recip ?
      unsigned int flags = 0;
      if (i > 0 && !m_phrases[i - 1].m_sentByMe)
      { 
        flags |= GuiRect::AMJU_TL | GuiRect::AMJU_TR;
      }
      // Next msg from recip ?
      if (i < (int)m_phrases.size() - 1 && !m_phrases[i + 1].m_sentByMe)
      {
        flags |= GuiRect::AMJU_BL | GuiRect::AMJU_BR;
      }
      r.SetRoundCorners(flags);
      
      r.Draw();
    }

    text->Draw();
  }

  // Draw name tag at top
  GuiRect r;
  r.SetLocalPos(Vec2f(0.3f, 1.0f));
  r.SetSize(Vec2f(0.6f, 0.12f));
  r.SetCornerRadius(0.04f);
  r.SetColour(Colour(0, 0, 0, 1)); 
  r.SetRoundCorners(GuiRect::AMJU_TL | GuiRect::AMJU_TR);
  r.Draw();

  GuiText text;
  text.SetLocalPos(Vec2f(0.3f, 1.0f));
  text.SetSize(Vec2f(0.6f, 0.12f)); 
  text.SetTextSize(1.0f);
  text.SetText(cc->m_recipName);
  text.SetDrawBg(false);
  text.SetJust(GuiText::AMJU_JUST_CENTRE);
  text.SetFgCol(Colour(1, 1, 1, 1));
  text.Draw();
}

void ChatConsole::Conversation::AddText(bool sentNotRecv, const std::string& msg)
{
  static ChatConsole* cc = TheChatConsole::Instance();

  GuiText* text = new GuiText;
  text->SetIsMulti(true); // So we can see all of a long msg!
  text->SetSize(Vec2f(cc->m_size.x, cc->m_fontSize * GuiText::CHAR_HEIGHT_FOR_SIZE_1)); 
  text->SetTextSize(cc->m_fontSize);
  text->SetText(msg);
  text->SetSize(Vec2f(cc->m_size.x, (float)text->GetNumLines() * cc->m_fontSize * GuiText::CHAR_HEIGHT_FOR_SIZE_1)); 
  text->SetDrawBg(false);
  text->SetInverse(false);
  if (sentNotRecv)
  {
    text->SetJust(GuiText::AMJU_JUST_LEFT);
    text->SetBgCol(Colour(1, 1, 1, 1));
    text->SetFgCol(Colour(0, 0, 0, 1));
  }
  else
  {
    text->SetJust(GuiText::AMJU_JUST_RIGHT);
    text->SetBgCol(Colour(1, 0, 1, 1)); // TODO different colours for different recips ?
    text->SetFgCol(Colour(1, 1, 1, 1));
  }

  m_phrases.push_back(Phrase(sentNotRecv, text));
}

ChatConsole::ChatConsole()
{
  m_lastRecipId = -1;
  m_chatSendIsActive = false;
  m_chatRecvIsActive = false;

  // TODO Load from config
  m_size = Vec2f(0.8f, 1.0f); 
  m_pos = Vec2f(1.0f - m_size.x, -1.0f);
  m_bottomY = -1.0f; // KB moves this up
  m_fontSize = 0.7f; // TODO CONFIG
  
  m_vel = Vec2f(-10.0f, 0); // moves from right hand side

  m_typing = false;
}

void ChatConsole::SetBottom(float y)
{
  m_pos.y = y;

  Vec2f pos = m_gui->GetLocalPos();
  pos.y = y;
  m_gui->SetLocalPos(pos);
}

void ChatConsole::Hide()
{
  m_mode = CHAT_CLOSED;
  Vec2f pos = m_gui->GetLocalPos();
  pos.x = 1.0f;
  m_gui->SetLocalPos(pos);
  m_gui->SetVisible(false);

  static GSMain* gsm = TheGSMain::Instance();
  gsm->SetViewWidth(1.0f);
}

void ChatConsole::Update()
{
  if (!m_gui)
  {
    return;
  }

  TheKb::Instance()->Update();

  static GSMain* gsm = TheGSMain::Instance();

  switch (m_mode)
  {
  case CHAT_OPENING:
    {
    m_gui->SetVisible(true);

    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_gui->GetLocalPos();
    pos += m_vel * dt;

    if (pos.x < m_pos.x)
    {
      pos.x = m_pos.x;
      m_mode = CHAT_OPEN;
    }
    gsm->SetViewWidth((pos.x + 1.0f) * 0.5f);
    m_gui->SetLocalPos(pos);
    }
    break;

  case CHAT_CLOSING:
    {
    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_gui->GetLocalPos();
    pos -= m_vel * dt;
    if (pos.x > 1.0f)
    {
      pos.x = 1.0f;
      m_mode = CHAT_CLOSED;
      m_gui->SetVisible(false);
    }
    gsm->SetViewWidth((pos.x + 1.0f) * 0.5f);
    m_gui->SetLocalPos(pos);
    }
    break;

  case CHAT_OPEN:
    if (m_typing)
    {
      static float timer = 0;
      static int t = 0;
      static const char* DOTS[3] = { "  .", " . ", ".  " };
      timer += TheTimer::Instance()->GetDt();
      if (timer > 0.5f) // TODO
      {
        timer = 0;
        t++;
        if (t > 2)
        {         
          t = 0;
        }
        ((GuiText*)m_gui->GetElementByName("chat-recip-name"))->SetText(DOTS[t] + m_recipName);
      }
    }
  default:
    break;
  }
}

void ChatConsole::Draw()
{
  static Kb* kb = TheKb::Instance();
  kb->Draw();

  if (m_gui && m_mode != CHAT_CLOSED)
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
  TheEventPoller::Instance()->RemoveListener(m_gui);
  m_gui = 0;
}

void ChatConsole::SetPlayerLoggedIn(Player* p, bool isLoggedIn)
{
  Assert(p);
  if (m_mode == CHAT_CLOSED)
  {
    return;
  }
  if (p->GetId() == m_lastRecipId)
  {
    std::string s = p->GetName() + " has just logged " + (isLoggedIn ? "in!" : "out!");
    TheLurker::Instance()->Queue(LurkMsg(s, Colour(1, 1, 1, 1), Colour(1, 0, 1, 1), AMJU_TOP));
  }
}

void ChatConsole::SetPlayerIsTyping(bool isTyping, int typerId, int recipId)
{
  // TODO
  m_typing = false;
  if (isTyping)
  {
std::cout << "This player: " << typerId << " is typing a message for " << recipId << ".\n";
    if (typerId == m_lastRecipId && recipId == GetLocalPlayerId())
    {
std::cout << "That's ME!!\n";
       m_typing = true;
    }
  }
  else
  {
std::cout << "This player: " << typerId << " is not typing anything.\n";
    if (typerId == m_lastRecipId)
    {
      // Hide dots
      ((GuiText*)m_gui->GetElementByName("chat-recip-name"))->SetText(m_recipName);
    }
  }
}

void OnTyping()
{
std::cout << "Typing!!\n";
  TheChatConsole::Instance()->OnTyping();
}

void ChatConsole::OnTyping()
{
  if (!m_gui)
  {
std::cout << "Unexpected, tried to access ChatConsole: OnTyping\n";
    return;
  }

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
  textedit->SetOnChangeFunc(Amju::OnTyping);

  GetElementByName(m_gui, "chat-cancel-button")->SetCommand(Amju::OnChatCancelButton);
  ////GetElementByName(m_gui, "recv-close-button")->SetCommand(Amju::OnRecvCloseButton);
  ActivateChatSend(false, -1);
  ActivateChatRecv(false);

  Hide();
}

void ChatConsole::OnChatSend()
{
  if (!m_gui)
  {
std::cout << "Unexpected, tried to access ChatConsole: OnChatSend\n";
    return;
  }

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
  OnTyping(); // empty text so flag will reset

////  GetElementByName(m_gui, "msg-recv-comp")->SetVisible(true);
}

void ChatConsole::OnChatCancel()
{
  // Chat console closed
  ActivateChatSend(false, -1);
}

//void ChatConsole::OnRecvClose()
//{
//  ActivateChatRecv(false);
//}

bool ChatConsole::CanShowMsg() const
{
  return true; // TODO !m_chatRecvIsActive; // && !m_chatSendIsActive ??
}

void ChatConsole::ActivateChatSend(bool active, int recipId)
{
  if (!m_gui)
  {
std::cout << "Unexpected, tried to access ChatConsole: ActivateChatSend\n";
    return;
  }

  // polish -- jump onto screen
  if (active && m_mode != CHAT_OPEN)
  {
    m_mode = CHAT_OPENING;
  }
  else if (!active && m_mode != CHAT_CLOSED)
  {
    m_mode = CHAT_CLOSING;
  }

  m_lastRecipId = recipId;

  if (active)
  {
std::cout << "Activate chat -- recip ID = " << recipId << "\n";

    GetNameForPlayer(recipId, &m_recipName);

    m_gui->GetElementByName("chat-comp")->SetVisible(true);

    GuiText* guiRecipName = ((GuiText*)m_gui->GetElementByName("chat-recip-name"));
    guiRecipName->SetText(m_recipName);

    GuiButton* send = (GuiButton*)GetElementByName(m_gui, "chat-send-button");
    send->SetHasFocus(true); // use in preference to SetIsFocusButton

    GuiTextEdit* textedit = (GuiTextEdit*)GetElementByName(m_gui, "chat-text-edit");
    textedit->SetHasFocus(true);
    //textedit->SetText("");

    GuiElement* bg = GetElementByName(m_gui, "bg");
    Vec2f size = bg->GetSize();
    Vec2f pos = bg->GetLocalPos();

    // Soft KB: set size of GUI depending on KB enabled/disabled
    static Kb* kb = TheKb::Instance();
    if (kb->IsEnabled())
    {
      kb->Load("gui-kb-email-lower.txt"); // TODO

      kb->Activate();
      SetBottom(-0.2f); 
      size.y = 1.2f;
      bg->SetSize(size);
      pos.y = 1.2f;
      bg->SetLocalPos(pos);

      guiRecipName->SetLocalPos(Vec2f(guiRecipName->GetLocalPos().x, 1.2f));
    }
    else
    {
      SetBottom(-1.0f); 
      size.y = 2.0f;
      bg->SetSize(size);
      pos.y = 2.0f;
      bg->SetLocalPos(pos);

      guiRecipName->SetLocalPos(Vec2f(guiRecipName->GetLocalPos().x, 2.0f));
    }
  }
  else
  {
    TheKb::Instance()->Deactivate();
    //m_gui->GetElementByName("chat-comp")->SetVisible(false);
  }
  m_chatSendIsActive = active;
}

void ChatConsole::ActivateChatRecv(bool active, const MsgManager::Msg* msg)
{
  if (!m_gui)
  {
std::cout << "Unexpected, tried to access ChatConsole: ActivateChatRecv\n";
    return;
  }

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

    static std::set<int> displayedIDs;
    if (displayedIDs.count(msg->m_id))
    {
std::cout << "Wait, we already displayed this msg!!\n"; 
      Assert(0);
    }
    displayedIDs.insert(msg->m_id);

    m_convs[m_lastRecipId].AddText(false, msg->m_text);

  }

////  GetElementByName(m_gui, "msg-recv-comp")->SetVisible(active);
  m_chatRecvIsActive = active;
}

}

