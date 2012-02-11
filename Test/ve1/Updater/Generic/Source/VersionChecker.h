#ifndef VERSION_CHECKER_H_INCLUDED
#define VERSION_CHECKER_H_INCLUDED

#include <OnlineReqManager.h>

namespace Amju
{
class VersionChecker : public OnlineReq
{
public:
  VersionChecker(const std::string& url) : OnlineReq(url, HttpClient::GET, "version checker")
  {
  }

  virtual void HandleResult();
};

}


#endif

