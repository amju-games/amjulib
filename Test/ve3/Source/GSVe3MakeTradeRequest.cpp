#include <GuiTextEdit.h>
#include <Game.h>
#include "GSVe3MakeTradeRequest.h"
#include "GSVe3ViewOtherPlayers.h"
#include "LurkMsg.h"
#include "LocalPlayer.h"
#include "MsgManager.h"
#include "GameConsts.h"

namespace Amju
{
GSVe3MakeTradeRequest::GSVe3MakeTradeRequest()
{
  m_showLurk = true;
  m_tradeType = TRADE_NONE;
}

void GSVe3MakeTradeRequest::Update()
{
  GSGui::Update();
  m_spriteNode.Update();
  TheLurker::Instance()->Update();
}

void GSVe3MakeTradeRequest::Draw()
{
  GSGui::Draw();
}

void GSVe3MakeTradeRequest::Draw2d()
{
  // TODO Draw ch we want to trade with

  GSGui::Draw2d();
}

static void OnTradeCancel()
{
  TheGSVe3MakeTradeRequest::Instance()->GoBack();
}

static void OnTradeSend()
{
  TheGSVe3MakeTradeRequest::Instance()->OnTradeSend();
}

static void OnTradeDoneOk()
{
  // TODO What state should we go to - View Other Players, right?
//  TheGSVe3MakeTradeRequest::Instance()->GoBack();

  TheGame::Instance()->SetCurrentState(TheGSVe3ViewOtherPlayers::Instance());
}

void GSVe3MakeTradeRequest::OnTradeSend()
{
  // Make a message: add a special code to make it easier to deal with 
  //  on the recving end.

  // TODO messages need to contain the info about the trade!!!
  GuiTextEdit* give = (GuiTextEdit*)GetElementByName(m_gui, "edit-num-to-give");
  GuiTextEdit* recv = (GuiTextEdit*)GetElementByName(m_gui, "edit-num-to-recv");
  GuiTextEdit* greet = (GuiTextEdit*)GetElementByName(m_gui, "edit-greet");

  int nGive = ToInt(give->GetText());
  int nRecv = ToInt(recv->GetText());
  // Sanity check
  if (nGive < 1 || nRecv < 1)
  {
    LurkMsg lm("No way! Please check your numbers!", LURK_FG, LURK_BG, AMJU_CENTRE); 
    TheLurker::Instance()->Queue(lm);    
    return;
  }

  // Check you have that much
  Player* p = GetLocalPlayer();
  std::string lpname = p->GetName();

  int maxToGive = ToInt((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? p->GetVal(FOOD_STORED_KEY) : p->GetVal(TREASURE_KEY));
  if (nGive > maxToGive)
  {
    std::string str = "You only have " + ToString(maxToGive) + 
      ((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? " food!" : " treasure!");
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE); 
    TheLurker::Instance()->Queue(lm);
    return;
  }

  std::string pn = m_otherTradePlayer->GetName(); 

  std::string str = "Hi " + pn + "! ";
  if (m_tradeType == TRADE_FOOD_FOR_TREASURE)
  {
    str += "I would like to trade " + ToString(nGive) + " of my food for " + ToString(nRecv) + " of your treasure. How about it? " +
      greet->GetText() + " from " + lpname;
  }
  else if (m_tradeType == TRADE_TREASURE_FOR_FOOD)
  {
    str += "I would like to trade " + ToString(nGive) + " of my treasure for " + ToString(nRecv) + " of your food. How about it? " +
      greet->GetText() + " from " + lpname;
  }
  else
  {
    Assert(0);
    return;
  }

  MsgManager* mm = TheMsgManager::Instance();
  mm->SendTradeMsg(p->GetId(), m_otherPlayerId, str, nGive, nRecv, m_tradeType); 
    // sender, recip, msg, give num, recv num, trade type

  // Also a message in our own guestbook?
  str = "You asked " + pn + " to make a trade.";
  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg
  // TODO Get this msg into guestbook - send a request for new msgs

  str = "OK, now let's wait and see what reply you get!";
  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeDoneOk); 
  TheLurker::Instance()->Queue(lm);    
}

void GSVe3MakeTradeRequest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-maketraderequest.txt");
  Assert(m_gui);

  Player* p = GetLocalPlayer();

  Assert(m_tradeType != TRADE_NONE);
  Assert(m_otherTradePlayer);

  std::string pn = m_otherTradePlayer->GetName(); 
  GuiText* t = (GuiText*)GetElementByName(m_gui, "static-text2");
  t->SetText("You can ask " + pn + 
    " if they want to make a trade,"
    " by sending a message:");

  t = (GuiText*)GetElementByName(m_gui, "msg-text1");
  t->SetText("Hi, " + pn + "!");

  t = (GuiText*)GetElementByName(m_gui, "msg-text6");
  t->SetText("from " + p->GetName());

  t = (GuiText*)GetElementByName(m_gui, "msg-text2");
  GuiText* give = (GuiText*)GetElementByName(m_gui, "msg-text-to-give");
  GuiText* recv = (GuiText*)GetElementByName(m_gui, "msg-text-to-recv");

  switch (m_tradeType)
  {
  case TRADE_NONE:
    Assert(0);
    break;
  case TRADE_FOOD_FOR_TREASURE:
    t->SetText("I would like to trade some of my food for some of your treasure.");
    give->SetText("food.");
    recv->SetText("treasure.");
    break;
  case TRADE_TREASURE_FOR_FOOD:
    t->SetText("I would like to trade some of my treasure for some of your food.");
    give->SetText("treasure.");
    recv->SetText("food.");
    break;
  }

  GetElementByName(m_gui, "back-button")->SetCommand(Amju::OnTradeCancel);
  GetElementByName(m_gui, "send-button")->SetCommand(Amju::OnTradeSend);
}

}
