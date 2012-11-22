#include <EventPoller.h>
#include "BroadcastConsole.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "ROConfig.h"

namespace Amju
{
static void OnEnter()
{
  TheBroadcastConsole::Instance()->OnEnter();
}

BroadcastConsole::BroadcastConsole()
{
  m_edit = new GuiTextEdit;
  m_edit->SetLocalPos(Vec2f(-1.0f, -0.9f)); // TODO depends on KB, and can move
  m_edit->SetSize(Vec2f(2.0f, 0.1f)); // TODO depends on chat console
  m_edit->SetBgCol(Colour(0, 0, 0, 1));
  m_edit->SetFgCol(Colour(1, 1, 1, 1));

  TheEventPoller::Instance()->AddListener(m_edit);
  m_edit->SetCommand(Amju::OnEnter);
}

void BroadcastConsole::OnEnter()
{
  std::string str = m_edit->GetText();
std::cout << "Broadcast console got enter key press.. msg is: \"" << str << "\"\n";
  if (!str.empty())
  {
    m_edit->SetText("");
    static const int BROADCAST = -2;
    TheMsgManager::Instance()->SendMsg(GetLocalPlayerId(), BROADCAST, str);
  }
}

void BroadcastConsole::Update()
{
}

void BroadcastConsole::Draw()
{
  for (Texts::iterator it = m_texts.begin(); it != m_texts.end(); ++it)
  {
    RCPtr<GuiText> text = *it;
    text->Draw();
  }

  m_edit->Draw();
}

void BroadcastConsole::OnMsgRecv(const std::string& str)
{
  GuiText* text = new GuiText;
  text->SetIsMulti(true);
  text->SetTextSize(1.0f); // TODO CONFIG
  text->SetSize(Vec2f(1.6f, 0.1f)); // assume single line
  text->SetText(str);
  text->SizeToText();
  text->SetFgCol(Colour(1, 1, 0, 1));

  m_texts.push_front(text);

  static const unsigned int NUM_LINES = ROConfig()->GetInt("chat-max-lines", 10);
  if (m_texts.size() > NUM_LINES) 
  {
    m_texts.pop_back();
  }

  // Iterate, set positions
  float x = -1.0f;
  float y = -0.8f; // TODO Bottom of screen after KB drawn
  for (Texts::iterator it = m_texts.begin(); it != m_texts.end(); ++it)
  {
    RCPtr<GuiText> text = *it;
    text->SetLocalPos(Vec2f(x, y));
    y += text->GetSize().y;
  }

}

}
