#ifndef SERIAL_REQ_MANAGER_H_INCLUDED
#define SERIAL_REQ_MANAGER_H_INCLUDED

#include <OnlineReqManager.h>

namespace Amju
{
class SerialReqManager
{
public:
  SerialReqManager();

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

  // Count all requests
  int CountAllReqs() const;

private:
  typedef std::deque<RCPtr<OnlineReq> > OnlineReqs;
  OnlineReqs m_reqs;

  // Worker thread which gets the front request using m_hc, then sleeps
  class SerialThread : public Thread
  {
  public:
    virtual void Work();
    void SetReq(RCPtr<OnlineReq> req);
    bool Waiting() const;

  private:
    RCPtr<OnlineReq> m_req;
    HttpClient m_hc;
    mutable Mutex m_mutex;
  };
  SerialThread* m_thread;
};
}

#endif
