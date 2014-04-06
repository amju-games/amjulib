#include <GuiText.h>
#include "GSVe3MakeTradeRequest.h"
#include "LurkMsg.h"
#include "LocalPlayer.h"
#include "MsgManager.h"

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

void GSVe3MakeTradeRequest::OnTradeSend()
{
  // Make a message: add a special code to make it easier to deal with 
  //  on the recving end.

  Player* p = GetLocalPlayer();

  std::string msg = p->GetName() + " gave some food to " + m_player->GetName() + "!";
  MsgManager* mm = TheMsgManager::Instance();
  mm->SendMsg(p->GetId(), m_player->GetId(), msg);
  // Also a message in our own guestbook?
  std::string str = "You gave some food to " + m_player->GetName() + "!";
  mm->SendMsg(m_player->GetId(), p->GetId(), str);
  // TODO Get this msg into guestbook - send a request for new msgs

}

void GSVe3MakeTradeRequest::SetPlayer(Player* p)
{
  m_player = p;
}

void GSVe3MakeTradeRequest::SetTradeType(TradeType tt)
{
  m_tradeType = tt;
}

void GSVe3MakeTradeRequest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-maketraderequest.txt");
  Assert(m_gui);

  Assert(m_tradeType != TRADE_NONE);
  Assert(m_player);

  std::string pn = m_player->GetName(); 
  GuiText* t = (GuiText*)GetElementByName(m_gui, "static-text2");
  t->SetText("You can ask " + pn + 
    " if they want to trade some of their food for some of your treasure,"
    " by sending a message:");

  t = (GuiText*)GetElementByName(m_gui, "msg-text1");
  t->SetText("Hi, " + pn + "!");

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
