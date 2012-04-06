#ifndef CHAT_CONSOLE_H_INCLUDED
#define CHAT_CONSOLE_H_INCLUDED

#include <map>
#include <vector>
#include <Singleton.h>
#include <EventListener.h>
#include <GuiText.h>
#include "MsgManager.h"

namespace Amju
{
// The user interface for chatting with other players
class ChatConsole : public NonCopyable, public EventListener
{
private:
  ChatConsole();
  friend class Singleton<ChatConsole>;

public:
  bool  IsActive() const;

  void Draw();
  void Update(); // ?

  void OnActive();
  void OnDeactive(); 

  //virtual bool OnKeyEvent(const KeyEvent&);
  //virtual bool OnCursorEvent(const CursorEvent&);
  //virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  bool CanShowMsg() const;
//  void ShowMsg(const MsgManager::Msg& msg);
  void OnChatSend();
  void OnChatCancel();
  void ActivateChatSend(bool active, int recipId);
  void ActivateChatRecv(bool active, const MsgManager::Msg* = 0);
  void OnRecvClose();

private:
  PGuiElement m_gui;

  // Chat recipient
  int m_lastRecipId;
  bool m_chatSendIsActive;
  bool m_chatRecvIsActive;

  // One conversation is between two players, comprised of messages
  struct Conversation 
  {
    std::vector<RCPtr<GuiText> > m_texts;

    void Draw();
    void AddText(bool sentNotRecv, const std::string& msg);
  };

  typedef std::map<int, Conversation> Convs;
  Convs m_convs;
};

typedef Singleton<ChatConsole> TheChatConsole;
}

#endif

