#include <Game.h>
#include <GuiScroll.h>
#include <GuiText.h>
#include <GuiRect.h>
#include <GuiButton.h>
#include <TimePeriod.h>
#include "TimePeriodString.h" // TODO promote to amjulib
#include "GuestbookWindow.h"
#include "MsgManager.h"
#include "Player.h"
#include "Mugshot.h"
#include "ObjectManager.h"
#include "GSVe3MsgReply.h"
#include "GSVe3ShowTrade.h"
#include "LocalPlayer.h"

namespace Amju
{
class ReplyTradeCommand : public GuiCommand
{
public:
  ReplyTradeCommand(int msgId) : m_msgId(msgId) {}

  bool Do() override
  {
    Game* game = TheGame::Instance();
    GSVe3ShowTrade* st = TheGSVe3ShowTrade::Instance();
    st->SetMsgId(m_msgId);
    // Set state to go back to
    st->SetPrevState(game->GetState());
    game->SetCurrentState(st);

    return false; // no undo
  };

private:
  int m_msgId;
};

class ReplyCommand : public GuiCommand
{
public:
  ReplyCommand(int msgId) : m_msgId(msgId) {}

  bool Do() override
  {
    Game* game = TheGame::Instance();
    GSVe3MsgReply* mr = TheGSVe3MsgReply::Instance();
    mr->SetMsgId(m_msgId);
    // Set state to go back to
    mr->SetPrevState(game->GetState());
    game->SetCurrentState(mr);

    return false; // no undo
  }

private:
  int m_msgId;
};

GuiElement* CreateGuestbookWindow()
{
  GuestbookWindow* w = new GuestbookWindow;
  GBDisplay* gbd = new GBDisplay;
  GuiScroll* s = new GuiScroll;
  w->AddChild(s);
  s->AddChild(gbd);

  return w;
}

GuiScroll* GuestbookWindow::GetScroll()
{
  GuiElement* child = GetChild(0);
  Assert(child);
  Assert(dynamic_cast<GuiScroll*>(child));
  return (GuiScroll*)child;
}

GBDisplay* GuestbookWindow::GetGBDisplay()
{
  GuiScroll* sc = GetScroll();
  GuiElement* child = sc->GetChild(0);
  Assert(dynamic_cast<GBDisplay*>(child));
  return (GBDisplay*)child;
}

void GBDisplay::Init(const MsgManager::Msgs& msgs, bool addReplyButtons)
{
  Clear();

  GuiElement* scroll = GetParent();
  Assert(scroll);
  GuiElement* window = scroll->GetParent();
  Assert(window);

  // BG - first so drawn under
  GuiRect* rect = new GuiRect;
  float g = 0.9f; // grey 
  rect->SetColour(Colour(g, g, g, 1));
  // round corners don't work well here - only the top left round corner is visible.
  //rect->SetRoundCorners(0x00); 
  //rect->SetCornerRadius(0.05f);

  AddChild(rect);

  Vec2f size;
  float y = 0;
  for (auto it = msgs.rbegin(); it != msgs.rend(); ++it) // reverse so we put latest message at top
  {
    MsgManager::Msg msg = it->second;
    Time timestamp = it->first;

    // Add one row
    GuiComposite* comp = new GuiComposite;

    // Add mugshot for sender
    int senderId = msg.m_senderId;
    int recipId = msg.m_recipId;

    std::string senderName = "System";
    bool isMsg = true;

    if (senderId <= 0)
    {
      senderId = recipId;
      isMsg = false; // not a message sent by a player
    }

    if (recipId < 0)
    {
      // Broadcast msg
      isMsg = false;
    }

    GuiWindow* w = new GuiWindow;
    comp->AddChild(w);
    w->SetSize(Vec2f(0.15f, 0.2f)); // TODO
    w->SetLocalPos(Vec2f(0.05f, -0.05f));

    // TODO Differnece between actual messages sent by people and messages generated by events
    //  like giving food etc.
    Player* sender = dynamic_cast<Player*>(TheObjectManager::Instance()->GetGameObject(senderId).GetPtr());
    if (sender)
    {
      if (senderId == GetLocalPlayerId())
      {
        senderName = "you"; // NB Case, sigh
      }
      else
      {
        senderName = sender->GetName();
      }

      Mugshot* m = new Mugshot;
      m->Init(sender);
      w->AddChild(m);
    }

    // Add message
    GuiText* label = new GuiText;
    label->SetName("label");
    label->SetIsMulti(true);
    label->SetFgCol(Colour(0, 0, 0, 1)); 
    label->SetFontSize(0.7f);
    label->SetSize(Vec2f(1.5f, 0.1f)); // horizontal size in text file - TODO get it from Window
    label->SetJust(GuiText::AMJU_JUST_LEFT);

    // How long ago?
    TimePeriod period = Time::Now() - timestamp;
    bool doCapitalise = true;
    bool isPastTense = true;
    std::string when = InformalTimePeriodString(period, doCapitalise, isPastTense);
    // TODO Show time if today, else date + time ?
    when += " (" + timestamp.ToString() + ")";

    std::string s;
    
    if (isMsg && 
        senderId != recipId) // ? It looks stupid
    {
      s = "Msg from " + senderName;
    
      // TODO flag for this
      Player* recip = dynamic_cast<Player*>(TheObjectManager::Instance()->GetGameObject(recipId).GetPtr());
      if (recip)
      {
        s += " To " + recip->GetName();
      }

      s += " Sent: " + when + "\n" + msg.GetStrippedText();
    }
    else
    {
      s = when + "\n" + msg.GetStrippedText();
    }

    label->SetText(s);
    label->SizeToText();

    comp->AddChild(label);

    bool canReply = isMsg;
    if (senderId == recipId)
    {
      canReply = false;
    }

    if (addReplyButtons && canReply)
    {
      // Add reply button
      GuiButton* reply = new GuiButton;
      reply->OpenAndLoad("reply-button.txt"); 
      if (msg.IsTrade())
      {
        reply->SetCommand(new ReplyTradeCommand(msg.m_id));
      } 
      else
      {
        reply->SetCommand(new ReplyCommand(msg.m_id));
      }

      // TODO This positions the button correctly but not the text inside!
      reply->SetLocalPos(Vec2f(0.3f, -0.05f - label->GetSize().y));
      
      comp->AddChild(reply);
    }

    // Set pos - y is number of rows
    comp->SetSizeFromChildren(); // ? Or work it out here
    // Set pos after size calc
    label->SetLocalPos(Vec2f(0.3f, -0.05f)); // TODO

    // Keep track of overall size for scrolling
    Vec2f compsize = comp->GetSize();
    if (compsize.x > size.x)
    {
      size.x = compsize.x;
    }
    if (compsize.y > size.y)
    {
      size.y = compsize.y;
    }

    comp->SetLocalPos(Vec2f(0, y)); 
    y -= (compsize.y + 0.05f); // TODO CONFIG

    AddChild(comp);
  }  
  Vec2f size2 = GetSize();
  size2.x = window->GetSize().x;
  size2.y = -y + 0.4f; 
  SetSize(size2);
  
  rect->SetSize(Vec2f(100, 100)); // TODO TEMP "FIX"

  scroll->SetSize(window->GetSize());
}

}
