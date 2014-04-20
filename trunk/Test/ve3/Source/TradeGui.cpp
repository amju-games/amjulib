#include "TradeGui.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"

namespace Amju
{
TradeGui::TradeGui()
{
  Reset();
}

void TradeGui::Reset()
{
  // Values we can sanity check later
  m_otherTradePlayer = nullptr;
  m_otherPlayerId = -1;
  m_msgId = -1;
  m_give = -1;
  m_recv = -1;
  m_tradeType = TRADE_NONE;
}

void TradeGui::SetMsgId(int msgId)
{
  Assert(m_msgId == -1); // Set already with no reset
  Assert(m_give == -1);
  Assert(m_recv == -1);
  Assert(m_tradeType == TRADE_NONE);

  m_msgId = msgId;

  // Set trade info from msg
  MsgManager* mm = TheMsgManager::Instance();
  const MsgManager::Msg* msg = mm->GetMsg(m_msgId);
  Assert(msg);

  m_otherPlayerId = msg->m_senderId;

  m_otherTradePlayer = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(m_otherPlayerId).GetPtr());
  Assert(m_otherTradePlayer);

  msg->GetTradeInfo(&m_give, &m_recv, &m_tradeType);
}

void TradeGui::SetTradeType(TradeType tt)
{
  Assert(m_tradeType == TRADE_NONE);

  m_tradeType = tt;
}

void TradeGui::SetGiveRecv(int give, int recv)
{
  Assert(m_give == -1);
  Assert(m_recv == -1);

  m_give = give;
  m_recv = recv;
}

void TradeGui::SetOtherPlayer(int playerId)
{
  // Sanity check not already set
  Assert(m_otherPlayerId == -1);
  Assert(!m_otherTradePlayer);

  m_otherPlayerId = playerId;

  m_otherTradePlayer = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(playerId).GetPtr());

  Assert(m_otherTradePlayer);
}

}
