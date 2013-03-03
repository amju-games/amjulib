#ifndef REQ_MSGREAD_H_INCLUDED
#define REQ_MSGREAD_H_INCLUDED

#include "Ve1Req.h"

namespace Amju 
{
class ReqMsgRead : public Ve1Req
{
public:
  ReqMsgRead(const std::string& url);

  virtual void OnSuccess();
};
} // namespace
#endif
