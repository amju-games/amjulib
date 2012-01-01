#include "ReqGetNewMsgs.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <SafeUtils.h>
#include "Timestamp.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include <StringUtils.h>

namespace Amju
{
ReqGetNewMsgs::ReqGetNewMsgs(const std::string& url) : Ve1Req(url, "getnewmsgs")
{
}

static std::string timestamp = "1";

void ReqGetNewMsgs::OnSuccess()
{
  PXml p = m_xml.getChildNode(0);
  Assert(SafeStrCmp(p.getName(), "now"));
  timestamp = p.getText();

  p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "msgs"))
  {
    int numMsgs = p.nChildNode();
    for (int i = 0; i < numMsgs; i++)
    {
      PXml msg = p.getChildNode(i);

      int id = atoi(msg.getChildNode(0).getText());
      int senderId = atoi(msg.getChildNode(1).getText());
      timestamp = msg.getChildNode(2).getText();
      Timestamp whenSent = atoi(timestamp.c_str());
      std::string text = msg.getChildNode(3).getText();

std::cout << "GOT NEW MSG!! ID: " << id << " sender: " << senderId << " sent: " << whenSent.ToString() << " text: " << text << "\n";

      TheMsgManager::Instance()->QueueMsg(MsgManager::Msg(id, text, senderId, whenSent));
    }
  }
  else
  {
std::cout << "Unexpected format for msgs\n";
  }
}

void SendGetNewMsgsReq()
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_MSGS);
  // TODO Add ID of last msg recved
  url += "&time=" + timestamp;
  url += "&recip=";
  url += ToString(GetLocalPlayer()->GetId());

std::cout << "Sending req to check for new msgs: " << url << "\n";

  // Only need one request in flight
  TheVe1ReqManager::Instance()->AddReq(new ReqGetNewMsgs(url), 1);
}

} // namespace
