/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: HttpReq.cpp,v $
Revision 1.5  2008/05/18 16:04:26  jay
MSVC update

Revision 1.4  2008/05/08 10:58:05  jay
New memory management code

Revision 1.3  2007/10/22 20:57:27  jay
Turn off debug printing the entire URL

Revision 1.2  2006/10/27 22:23:43  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.6  2006/08/27 19:19:07  jay
Turn on threads

Revision 1.1.2.5  2006/08/12 07:40:56  jay
Add #ifdef to debug output

Revision 1.1.2.4  2006/08/07 20:45:18  jay
Disable threads with NO_THREADS, for debugging.

Revision 1.1.2.3  2006/07/30 21:37:16  jay
Added CreateWorker for flexibility in subclasses: DON'T call this from ctor
 or it won't be virtualised! + Can turn off mutexes if really necessary.

Revision 1.1.2.2  2006/07/26 21:22:51  jay
More debug info

Revision 1.1.2.1  2006/07/25 08:15:47  jay
Added HttpReq to repository

*/

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include <iostream>
#include "HttpReq.h"
#include "AmjuFinal.h"

#ifdef _DEBUG
//#define HTTP_REQ_DEBUG
#endif

// Define to not use a separate thread for the request.
// For isolating threading issues, etc.
//#define NO_THREAD

// To be on the safe side we should use a mutex to sunch. access to
// the results.
// But if HttpReqs are always used correctly, we shouldn't actually
// need to lock the data.
// The correct way to access the data is t first check IsFinished(),
// and only access the other data if this returns true.
#define HTTP_REQ_USE_MUTEX

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

void HttpReq::CreateWorker()
{
  AMJU_CALL_STACK;

#ifdef NO_THREAD

#ifdef HTTP_REQ_DEBUG
std::cout << "NO THREAD, requesting URL: " << m_url.c_str() << "\n";
#endif

  HttpClient hc;
  HttpResult res = hc.Get(m_url, m_method);
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

  HttpResult res = hc.Get(m_url, m_method);

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


