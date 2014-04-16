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
  MsgManager* mm = TheMsgManager::Instance();

////  mm->SendTradeMsg(p->GetId(), m_player->GetId(), str, nGive, nRecv, m_tradeType); 
    // sender, recip, msg, give num, recv num, trade type

  Player* p = GetLocalPlayer();
  std::string lpname = p->GetName();

  //const MsgManager::Msg* msg = mm->GetMsg(m_msgId);
  //Assert(msg);
  //Player* other = dynamic_cast<Player*>(
  //  TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  std::string othername = m_otherTradePlayer->GetName();

  // Also a message in our own guestbook?
  // TODO Watch out - can we use "You" in a message??
  // (you can only if no-one else will see the msg.)
  //  Maybe use <sender> and <recip> and substitute when we display.
  std::string str = "You rejected the trade offered by " + othername + ".";
  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg

  str = lpname + " rejected your offer!";
  mm->SendMsg(p->GetId(), m_otherPlayerId, str); // sender, recip, msg

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
  MsgManager* mm = TheMsgManager::Instance();

  Player* p = GetLocalPlayer();
  std::string lpname = p->GetName();

  //const MsgManager::Msg* msg = TheMsgManager::Instance()->GetMsg(m_msgId);
  //Assert(msg);
  //Player* other = dynamic_cast<Player*>(
  //  TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  //// Get trade info. Make changes to counters for local player and other
  //int give = 0;
  //int recv = 0;
  //TradeType tt = TRADE_NONE;
  //msg->GetTradeInfo(&give, &recv, &tt);

  Assert(m_otherTradePlayer);
  std::string othername = m_otherTradePlayer->GetName();

  if (m_tradeType == TRADE_FOOD_FOR_TREASURE)
  {
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
