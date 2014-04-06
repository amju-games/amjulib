#include <GuiText.h>
#include "GSVe3MakeTradeRequest.h"
#include "LurkMsg.h"

namespace Amju
{
GSVe3MakeTradeRequest::GSVe3MakeTradeRequest()
{
  m_showLurk = true;
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
}


void GSVe3MakeTradeRequest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-maketraderequest.txt");
  Assert(m_gui);

  std::string pn = "<player name>";
  GuiText* t = (GuiText*)GetElementByName(m_gui, "static-text2");
  t->SetText("You can ask " + pn + " if they want to trade some of their food for some of your treasure,"
    " by sending a message:");

  GetElementByName(m_gui, "back-button")->SetCommand(OnTradeCancel);
  GetElementByName(m_gui, "send-button")->SetCommand(OnTradeSend);

}

}
