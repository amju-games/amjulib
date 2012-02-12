#ifndef VERSION_CHECKER_H_INCLUDED
#define VERSION_CHECKER_H_INCLUDED

#include <OnlineReqManager.h>

namespace Amju
{
class Updater;

class VersionChecker : public OnlineReq
{
public:
  VersionChecker(Updater* u, const std::string& url) : 
    OnlineReq(url, HttpClient::GET, "version checker"), m_updater(u)
  {
  }

  virtual void HandleResult();

private:
  Updater* m_updater;
};

}


#endif

