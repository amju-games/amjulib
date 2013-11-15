#ifndef REQ_SENDMSG_H_INCLUDED
#define REQ_SENDMSG_H_INCLUDED

#include "Ve1Req.h"

namespace Amju 
{
class ReqSendMsg : public Ve1Req
{
public:
  ReqSendMsg(const std::string& url);

  virtual void OnSuccess();
};
} // namespace
#endif
