#include "GSVe3ShowTrade.h"
#include "ShowMsgHelper.h"
#include "Player.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "HeartCount.h"

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

  const MsgManager::Msg* msg = TheMsgManager::Instance()->GetMsg(m_msgId);
  Assert(msg);
  Player* other = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  std::string othername = other->GetName();

  // Also a message in our own guestbook?
  // TODO Watch out - can we use "You" in a message??
  // (you can only if no-one else will see the msg.)
  //  Maybe use <sender> and <recip> and substitute when we display.
  std::string str = "You rejected the trade offered by " + othername + ".";
  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg

  str = lpname + " rejected your offer!";
  mm->SendMsg(p->GetId(), other->GetId(), str); // sender, recip, msg

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

  const MsgManager::Msg* msg = TheMsgManager::Instance()->GetMsg(m_msgId);
  Assert(msg);
  Player* other = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  std::string othername = other->GetName();

  // Get trade info. Make changes to counters for local player and other
  int give = 0;
  int recv = 0;
  TradeType tt = TRADE_NONE;
  msg->GetTradeInfo(&give, &recv, &tt);
  if (tt == TRADE_FOOD_FOR_TREASURE)
  {
    ChangePlayerCount(FOOD_STORED_KEY, give);
    ChangeObjCount(other->GetId(), FOOD_STORED_KEY, -give);

    ChangePlayerCount(TREASURE_KEY, -recv);
    ChangeObjCount(other->GetId(), TREASURE_KEY, recv);
  }
  else if (tt == TRADE_TREASURE_FOR_FOOD)
  {
    ChangePlayerCount(TREASURE_KEY, give);
    ChangeObjCount(other->GetId(), TREASURE_KEY, -give);

    ChangePlayerCount(FOOD_STORED_KEY, -recv);
    ChangeObjCount(other->GetId(), FOOD_STORED_KEY, recv);
  }
  else
  {
    Assert(0);
    return; // lurk msg ?
  }

  std::string str = "You accepted the trade offered by " + othername + "!";
  mm->SendMsg(p->GetId(), p->GetId(), str); // sender, recip, msg

  str = lpname + " accepted your offer!";
  mm->SendMsg(p->GetId(), other->GetId(), str); // sender, recip, msg

  str = "OK, you accepted the trade!";
  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnTradeCancel); 
  TheLurker::Instance()->Queue(lm);    
}

void GSVe3ShowTrade::OnTradeCounterOffer()
{
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
  ShowMsgHelper(m_msgId, m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(Amju::OnTradeCancel);
  GetElementByName(m_gui, "reject-button")->SetCommand(Amju::OnTradeReject);
  GetElementByName(m_gui, "accept-button")->SetCommand(Amju::OnTradeAccept);
  GetElementByName(m_gui, "counter-button")->SetCommand(Amju::OnTradeCounterOffer);

}

}
