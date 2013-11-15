#ifndef REQ_GETNEWMSGS_H_INCLUDED
#define REQ_GETNEWMSGS_H_INCLUDED

#include "Ve1Req.h"

namespace Amju 
{
class ReqGetNewMsgs : public Ve1Req
{
public:
  ReqGetNewMsgs(const std::string& url);

  virtual void OnSuccess();
};

void SendGetNewMsgsReq();

} // namespace
#endif
