#ifndef REQ_LOGIN_H_INCLUDED
#define REQ_LOGIN_H_INCLUDED

#include "Ve1Req.h"

namespace Amju 
{
class ReqLogin : public Ve1Req
{
public:
  ReqLogin(const std::string& url);

  virtual void OnSuccess();
  virtual void OnFailure();
};

void SendLoginReq(const std::string& email);

} // namespace
#endif
