#include "ReqMsgRead.h"
#include <iostream>

namespace Amju
{
ReqMsgRead::ReqMsgRead(const std::string& url) : Ve1Req(url, "msgread")
{
}

void ReqMsgRead::OnSuccess()
{
  std::cout << "Successfully marked msg as ok to delete on server.\n";
}

} // namespace
