#include <AmjuFirst.h>
#include "ReqSendMsg.h"
#include <iostream>
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

}

} // namespace
