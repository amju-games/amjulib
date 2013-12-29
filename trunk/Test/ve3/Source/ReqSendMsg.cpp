#include <AmjuFirst.h>
#include <iostream>
#include "ReqSendMsg.h"
#include "MsgManager.h"
#include <AmjuFinal.h>

namespace Amju
{
ReqSendMsg::ReqSendMsg(const std::string& url) : Ve1Req(url, "sendmsg")
{
}

void ReqSendMsg::OnSuccess()
{
// TODO
std::cout << "Successfully sent msg " << m_url << "\n";

  // TODO Should we check immediately?
  TheMsgManager::Instance()->CheckForNewMsgs();
}

} // namespace
