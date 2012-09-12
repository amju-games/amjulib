#include "ReqGetNewMsgs.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <SafeUtils.h>
#include "Timestamp.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include <StringUtils.h>
#include "GSNetError.h"

//#define SEND_DEBUG

namespace Amju
{
ReqGetNewMsgs::ReqGetNewMsgs(const std::string& url) : Ve1Req(url, "getnewmsgs")
{
  m_critical = false;
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
      text = DecodeMsg(text); ////Replace(text, "_", " "); // replace underscores with spaces -- TODO punctuation 

std::cout << "GOT NEW MSG!! ID: " << id << " sender: " << senderId << " sent: " << whenSent.ToString() << " text: " << text << "\n";

      TheMsgManager::Instance()->QueueMsg(MsgManager::Msg(id, text, senderId, whenSent));
    }
  }
  else
  {
    ShowError("Unexpected format for msgs response");
  }
}

void SendGetNewMsgsReq()
{
  if (!GetLocalPlayer())
  {
std::cout << "No local player yet, can't send get new msgs request\n";

    return;
  }

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_MSGS);
  // TODO Add ID of last msg recved
  url += "&time=" + timestamp;
  url += "&recip=";
  url += ToString(GetLocalPlayer()->GetId());

#ifdef SEND_DEBUG
std::cout << "Sending req to check for new msgs: " << url << "\n";
#endif

  // Only need one request in flight
  TheVe1ReqManager::Instance()->AddReq(new ReqGetNewMsgs(url), 1);
}

} // namespace
