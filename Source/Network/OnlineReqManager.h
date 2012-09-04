/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: OnlineReqManager.h,v $
Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.6  2006/08/12 07:40:20  jay
Changed AddReq() to return a bool, so we know if the max number of requests
was exceeded.

Revision 1.1.2.5  2006/08/07 20:46:11  jay
Fix iterator-invalidation bug

Revision 1.1.2.4  2006/07/30 21:38:07  jay
Count requests on queue with a given name; limit number of requests of a
  given name.

Revision 1.1.2.3  2006/07/26 21:19:16  jay
Pass SharedPtr, not raw ptr - seemed like a good idea but not really necessary

Revision 1.1.2.2  2006/07/25 17:45:38  jay
Fix bad call to erase; add GetName()

Revision 1.1.2.1  2006/07/25 08:16:34  jay
Added OnlineReqManager to repository

*/

#ifndef ONLINE_REQ_MANAGER_H_INCLUDED
#define ONLINE_REQ_MANAGER_H_INCLUDED

#include <deque>
#include "HttpReq.h"

namespace Amju
{
// Like an Http Request but with an extra function to handle the result
// once the request has completed.
class OnlineReq : public HttpReq
{
public:
  OnlineReq(
    const std::string& url, 
    HttpClient::HttpMethod method, 
    const std::string& name);

  virtual void Work();

  virtual void HandleResult() = 0;

  const std::string& GetName() const;

protected:
  // This is for debugging, so we can report on requests,
  // and can also be used to delete requests by name, or to count the
  // number of requests.
  std::string m_name;
}; 

// OnlineReqManager has a list of requests which are executed by other
// threads. Every Update() we poll the requests to see if they have
// finished. If so, we call HandleResult() on the request then delete it.
class OnlineReqManager
{
public:
  void Update();

  // Call to add a new request.
  // This will be executed by another thread. When the request has finished,
  // Update() will call HandleResult() on the request then erase it.
  // maxRequests: this is the max number of requests with the same name.
  // If adding the new request would exceed the maximum, the request
  // will not be added. 
  // Returns true if request added to queue; returns false if max number
  // of requests of this type would be exceeded.
  bool AddReq(SharedPtr<OnlineReq>, int maxRequestsOfThisType = 1);

  // Return the number of requests with the given name.
  int CountReqsWithName(const std::string& name);

  // Count all requests
  int CountAllReqs() const;

private:
  typedef std::deque<SharedPtr<OnlineReq> > OnlineReqs;
  OnlineReqs m_reqs;
};
}

#endif

