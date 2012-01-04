#ifndef VE1_REQ_H_INCLUDED
#define VE1_REQ_H_INCLUDED

#include <OnlineReqManager.h>
#include <Xml/XmlNodeInterface.h>

namespace Amju
{
// All requests should derive from this type, so we can always handle the response from the server
//  which says the login has timed out, etc.
// Here is an idea: The XML response is put into a factory to get a response object which knows how to behave..???

class Ve1Req : public OnlineReq
{
public:
  Ve1Req(const std::string url, const char* name) : OnlineReq(url, HttpClient::GET, name)
  {
  }

  // Sigh, this should be "final", so we always check for session timeout, etc.
  // Parse XML, checking for error msgs, then call OnSuccess/OnFailure
  virtual void HandleResult();

  virtual void OnSuccess() = 0;
  virtual void OnFailure();

protected:
  PXml m_xml;
  std::string m_errorStr;
};
}

#endif

