// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include <iostream>
#include "HttpReq.h"
#include <AmjuFinal.h>

#ifdef _DEBUG
//#define HTTP_REQ_DEBUG
#endif

// Define to not use a separate thread for the request.
// For isolating threading issues, etc.
//#define NO_THREAD

// To be on the safe side we should use a mutex to synch. access to
// the results.
// But if HttpReqs are always used correctly, we shouldn't actually
// need to lock the data.
// The correct way to access the data is to first check IsFinished(),
// and only access the other data if this returns true.
//#define HTTP_REQ_USE_MUTEX

namespace Amju
{
HttpReq::HttpReq(const std::string& url, HttpClient::HttpMethod method) :
  m_isFinished(false),
  m_url(url),
  m_method(method)
{
  AMJU_CALL_STACK;

}

HttpReq::~HttpReq()
{
  AMJU_CALL_STACK;

  // If the thread is still active, there is a reference to it held by
  // the ThreadManager. We call Stop() so when the thread does finish the
  // request it doesn't try to set members of this object, which will have
  // gone away.
  // If the thread has already finished, Stop() will set a flag but will
  // have no other effect. The last reference to the thread will then be
  // held by this object and so the thread will be deleted along with this.
  if (m_pThread.GetPtr())
  {
    m_pThread->Stop();
  }
}

void HttpReq::Work()
{
  HttpClient hc;

  DoRequest(hc);
}

void HttpReq::DoRequest(HttpClient& hc)
{
  HttpResult res;
  hc.Get(m_url, m_method, &m_httpResult);

  m_isFinished = true;
}

void HttpReq::CreateWorker()
{
  AMJU_CALL_STACK;

#ifdef NO_THREAD

#ifdef HTTP_REQ_DEBUG
std::cout << "NO THREAD, requesting URL: " << m_url.c_str() << "\n";
#endif

  HttpClient hc;
  HttpResult res;
  hc.Get(m_url, m_method);
  SetResult(res);

#ifdef HTTP_REQ_DEBUG
std::cout << "NO THREAD, got result: " << res.GetString().c_str() << "\n";
#endif

#else

  // Create a thread to do the request
  m_pThread = new HttpReqWorker(this, m_url, m_method); 
  m_pThread->Start();

#endif
}

bool HttpReq::IsFinished()
{
  AMJU_CALL_STACK;

#ifdef HTTP_REQ_USE_MUTEX
  MutexLocker m(m_mutex);
#endif
  return m_isFinished;
}

HttpResult HttpReq::GetResult()
{
  AMJU_CALL_STACK;

#ifdef HTTP_REQ_USE_MUTEX
  MutexLocker m(m_mutex);
#endif
  return m_httpResult; // copy it
}

void HttpReq::SetResult(const HttpResult& result)
{
  AMJU_CALL_STACK;

#ifdef HTTP_REQ_USE_MUTEX
  MutexLocker m(m_mutex);
#endif
  m_httpResult = result; 
  m_isFinished = true;
}

HttpReq::HttpReqWorker::HttpReqWorker(
  HttpReq* pReq,
  const std::string& url,
  HttpClient::HttpMethod method)
{
  AMJU_CALL_STACK;

  m_pReq = pReq;
  m_url = url;
  m_method = method;
}

void HttpReq::HttpReqWorker::Work()
{
  AMJU_CALL_STACK;

  HttpClient hc;

#ifdef HTTP_REQ_DEBUG
std::cout << "**WORK** Before Get, url: " << m_url.c_str() << "\n";
#endif

  HttpResult res;
  hc.Get(m_url, m_method, &res);

#ifdef HTTP_REQ_DEBUG
std::cout << "**WORK** After Get\n";
#endif

  if (m_stop)
  {
#ifdef HTTP_REQ_DEBUG
std::cout << "**WORK** Stop set ?!?! NOT SAFE to call SetFinished!\n";
#endif

    // Thread has been stopped - e.g. by the HttpReq going away.
    return;
  }

#ifdef HTTP_REQ_DEBUG
std::cout << "**WORK** Calling SetResult!\n";
#endif

  m_pReq->SetResult(res);

#ifdef HTTP_REQ_DEBUG
std::cout << "**WORK** Returning!!\n";
#endif
}
}


