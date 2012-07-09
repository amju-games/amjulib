#ifndef CHAT_CONSOLE_H_INCLUDED
#define CHAT_CONSOLE_H_INCLUDED

#include <map>
#include <vector>
#include <Singleton.h>
#include <EventListener.h>
#include <GuiText.h>
#include <Vec2.h>
#include "MsgManager.h"

namespace Amju
{
// The user interface for chatting with other players
class ChatConsole : public NonCopyable
{
private:
  ChatConsole();
  friend class Singleton<ChatConsole>;

public:
  bool  IsActive() const;

  void Draw();
  void Update(); 

  void OnActive();
  void OnDeactive(); 

  bool CanShowMsg() const;
  void OnChatSend();
  void OnChatCancel();
  void ActivateChatSend(bool active, int recipId);
  void ActivateChatRecv(bool active, const MsgManager::Msg* = 0);

  // Set flag for player typing msg to another player - display something if it makes sense
  void SetPlayerIsTyping(bool isTyping, int typerId, int recipId);

  // Called when we type into text edit box
  void OnTyping();

  // Set min y value:  -1 is bottom of screen.
  void SetBottom(float y); 

private:
  void Hide();

  PGuiElement m_gui;

  // Chat recipient
  int m_lastRecipId;
  std::string m_recipName;

  bool m_chatSendIsActive;
  bool m_chatRecvIsActive;

  // One conversation is between two players, comprised of messages
  struct Conversation 
  {
    std::vector<RCPtr<GuiText> > m_texts;

    void Draw();
    void AddText(bool sentNotRecv, const std::string& msg);
  };

  friend struct Conversation;

  typedef std::map<int, Conversation> Convs;
  Convs m_convs;

  // On screen pos, size
  Vec2f m_size; // size of entire chat window
  Vec2f m_pos; // top left of chat window
  float m_fontSize; // size of text for msgs

  enum Mode
  {
    CHAT_OPENING,
    CHAT_CLOSING,
    CHAT_OPEN,
    CHAT_CLOSED,
  };
  Mode m_mode;
  Vec2f m_vel; // vel when moving on screen

  bool m_typing;

  float m_bottomY;
};

typedef Singleton<ChatConsole> TheChatConsole;
}

#endif

