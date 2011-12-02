#ifndef VE1_REQ_H_INCLUDED
#define VE1_REQ_H_INCLUDED

#include <OnlineReqManager.h>

namespace Amju
{
// All requests should derive from this type, so we can always handle the response from the server
//  which says the login has timed out, etc.
// The XML response is put into a factory to get a response object which knows how to behave..???
class Ve1Req : public OnlineReq
{
public:
  Ve1Req(const std::string url, const char* name) : OnlineReq(url, HttpClient::GET, name)
  {
  }

  virtual void HandleResponse()
  {
    // TODO Check for timeout
  }
};
}

#endif

