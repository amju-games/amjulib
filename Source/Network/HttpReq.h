/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: HttpReq.h,v $
Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.2  2006/07/30 21:37:16  jay
Added CreateWorker for flexibility in subclasses: DON'T call this from ctor
 or it won't be virtualised! + Can turn off mutexes if really necessary.

Revision 1.1.2.1  2006/07/25 08:15:47  jay
Added HttpReq to repository

*/

#ifndef HTTP_REQ_H_INCLUDED
#define HTTP_REQ_H_INCLUDED

#include <Thread.h>
#include <Mutex.h>
#include "HttpClient.h"

namespace Amju
{
// This class encapsulates an Http client request: the url, the current
// state of the request, and the result once the request has completed. 
class HttpReq : public Shareable
{
public:
  // Creates a thread to handle the request.
  // The thread gets on with it, then sets the Finished flag and the
  // results once the request has finished.
  HttpReq(const std::string& url, HttpClient::HttpMethod m);

  virtual ~HttpReq();

  const std::string& GetUrl() const { return m_url; }

  // Create HttpClient, get response
  virtual void Work();

  // Use an existing HttpClient, do the request (url)
  void DoRequest(HttpClient& hc);

  // Call this to find out if the request has finished yet.
  // THREAD SAFE
  bool IsFinished();

  // Call this to get the result ONLY once IsFinished() returns true.
  // THREAD SAFE
  HttpResult GetResult();

//private: // But may be called by a worker class we don't know about.
  // Called by worker thread
  // THREAD SAFE
  void SetResult(const HttpResult& result);

protected:
  // Worker thread class
  // Allow this to be sublassed ?
  class HttpReqWorker : public Thread
  {
  public:
    HttpReqWorker(HttpReq* pReq, const std::string& url, HttpClient::HttpMethod m);
    virtual void Work();
  private:
    std::string m_url;
    HttpClient::HttpMethod m_method; 
    HttpReq* m_pReq;
  };
  // Allow the worker thread to set members
  friend class HttpReqWorker;

private:
  // Creates one of the above threads -- override to provide a special
  // kind of thread which does something else.
  // Called by OnlineReqManager when this request is successfully added.
  friend class OnlineReqManager; 
  virtual void CreateWorker();
 
protected:
  Mutex m_mutex; // lock IsFinished() and GetResult(), so our worker thread
   // can write to members while these functions are called from another thread.

  // This result is populated once the worker thread has finished.
  HttpResult m_httpResult;

  // This flag is set once our worker thread has finished.
  bool m_isFinished;

  // The worker thread
  SharedPtr<Thread> m_pThread;

  std::string m_url;
  HttpClient::HttpMethod m_method;
};
}

#endif

