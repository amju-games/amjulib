#include <Game.h>
#include "GSVe3ShowTrade.h"
#include "ShowMsgHelper.h"
#include "Player.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "HeartCount.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "GSVe3MakeTradeRequest.h"
#include "TradeFlags.h"

namespace Amju
{
static void OnTradeCancel()
{
  TheGSVe3ShowTrade::Instance()->GoBack();
}

static void OnTradeReject()
{
  TheGSVe3ShowTrade::Instance()->OnTradeReject();
}

void GSVe3ShowTrade::OnTradeReject()
{
  if (GetTradeResponse(m_msgId) != TRADE_REPLY_NONE)
  {
    std::string str = "You already replied to this trade offer!?";
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
    TheLurker::Instance()->Queue(lm);    
    return;
  }

  MsgManager* mm = TheMsgManager::Instance();
  Player* p = GetLocalPlayer();
  std::string lpname = p->GetName();
  std::string othername = m_otherTradePlayer->GetName();

  // Also a message in our own guestbook?
  // TODO Watch out - can we use "You" in a message??
  // (you can only if no-one else will see the msg.)
  //  Maybe use <sender> and <recip> and substitute when we display.

  // Not required - we tag each offer as accepted/rejected/etc
//  std::string str = "You rejected the trade offered by " + othername + ".";
//  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg

  std::string str = lpname + " rejected your offer!";
  mm->SendMsg(p->GetId(), m_otherPlayerId, str); // sender, recip, msg

  SetTradeResponse(m_msgId, TRADE_REPLY_REJECT);

  str = "OK, you rejected the trade.";
  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
  TheLurker::Instance()->Queue(lm);    
}

static void OnTradeAccept()
{
  TheGSVe3ShowTrade::Instance()->OnTradeAccept();
}

static void OnTradeCounterOffer()
{
  TheGSVe3ShowTrade::Instance()->OnTradeCounterOffer();
}

void GSVe3ShowTrade::OnTradeAccept()
{
  if (GetTradeResponse(m_msgId) != TRADE_REPLY_NONE)
  {
    std::string str = "You already replied to this trade offer!?";
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
    TheLurker::Instance()->Queue(lm);    
    return;
  }

  MsgManager* mm = TheMsgManager::Instance();

  Player* p = GetLocalPlayer();
  std::string lpname = p->GetName();

  // Check you have enough of the thing you are giving
  // Your max is your food count or treasure count, depending on the trade type
  int maxToGive = ToInt((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? 
    p->GetVal(TREASURE_KEY) :  // other player wants to give food, get treasure
    p->GetVal(FOOD_STORED_KEY));  // other player wants to give treasure, get food

  if (m_recv > maxToGive)
  {
    std::string str = "You only have " + ToString(maxToGive) + 
      ((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? " treasure!" : " food!");
    str += "\nYou can't make this trade right now!";

    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, Amju::OnTradeCancel); 
    TheLurker::Instance()->Queue(lm);
    return;
  }

  Assert(m_otherTradePlayer);
  std::string othername = m_otherTradePlayer->GetName();

  // Check the other player has enough of the thing you are recving!
  maxToGive = ToInt((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? 
    m_otherTradePlayer->GetVal(FOOD_STORED_KEY) :  // other player wants to give food, get treasure
    m_otherTradePlayer->GetVal(TREASURE_KEY));  // other player wants to give treasure, get food

  if (m_recv > maxToGive)
  {
    std::string str = "Oh no! " + othername + " only has " + ToString(maxToGive) + 
      ((m_tradeType == TRADE_FOOD_FOR_TREASURE) ? " food!" : " treasure!");
    str += "\nYou can't make this trade right now!";

    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, Amju::OnTradeCancel); 
    TheLurker::Instance()->Queue(lm);
    return;
  }


  // OK, this trade can be made!
  if (m_tradeType == TRADE_FOOD_FOR_TREASURE)
  {
    // Local player gets food. Other player gets treasure
    ChangePlayerCount(FOOD_STORED_KEY, m_give);
    ChangeObjCount(m_otherPlayerId, FOOD_STORED_KEY, -m_give);

    ChangePlayerCount(TREASURE_KEY, -m_recv);
    ChangeObjCount(m_otherPlayerId, TREASURE_KEY, m_recv);
  }
  else if (m_tradeType == TRADE_TREASURE_FOR_FOOD)
  {
    ChangePlayerCount(TREASURE_KEY, m_give);
    ChangeObjCount(m_otherPlayerId, TREASURE_KEY, -m_give);

    ChangePlayerCount(FOOD_STORED_KEY, -m_recv);
    ChangeObjCount(m_otherPlayerId, FOOD_STORED_KEY, m_recv);
  }
  else
  {
    Assert(0);
    return; // lurk msg ?
  }

  SetTradeResponse(m_msgId, TRADE_REPLY_ACCEPT);

  // Make string description of trade for the msgs we send

  std::string str = "You accepted the trade offered by " + othername + "!";
  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg

  str = lpname + " accepted your offer!";
  mm->SendMsg(p->GetId(), m_otherPlayerId, str); // sender, recip, msg

  str = "OK, you accepted the trade!";
  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
  TheLurker::Instance()->Queue(lm);    
}

void GSVe3ShowTrade::OnTradeCounterOffer()
{
  if (GetTradeResponse(m_msgId) != TRADE_REPLY_NONE)
  {
    std::string str = "You already replied to this trade offer!?";
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
    TheLurker::Instance()->Queue(lm);    
    return;
  }

  // Set up MakeTradeRequest page with counter offer
  GSVe3MakeTradeRequest* mtr = TheGSVe3MakeTradeRequest::Instance();
  mtr->Reset();
  // Set ID of other player
  mtr->SetOtherPlayer(m_otherPlayerId);

  // As we are making a counter offer, switch the trade type!
  mtr->SetTradeType(m_tradeType == TRADE_FOOD_FOR_TREASURE ? 
    TRADE_TREASURE_FOR_FOOD : TRADE_FOOD_FOR_TREASURE); 
  // .. and switch give/recv values!
  mtr->SetGiveRecv(m_recv, m_give);
  mtr->SetPrevState(GetPrevState()); // after sending counter offer, go back to our prev state

  SetTradeResponse(m_msgId, TRADE_REPLY_COUNTER);

  TheGame::Instance()->SetCurrentState(mtr);
}

GSVe3ShowTrade::GSVe3ShowTrade()
{
  m_showLurk = true;
  m_msgId = -1;
}

void GSVe3ShowTrade::Update()
{
  GSGui::Update();
}

void GSVe3ShowTrade::Draw()
{
  GSGui::Draw();
}

void GSVe3ShowTrade::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3ShowTrade::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-showtrade.txt");
  Assert(m_gui);

  Assert(m_msgId != -1);

  // Get info about trade from msg

  ShowMsgHelper(m_msgId, m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(Amju::OnTradeCancel);
  GetElementByName(m_gui, "reject-button")->SetCommand(Amju::OnTradeReject);
  GetElementByName(m_gui, "accept-button")->SetCommand(Amju::OnTradeAccept);
  GetElementByName(m_gui, "counter-button")->SetCommand(Amju::OnTradeCounterOffer);

}

}
