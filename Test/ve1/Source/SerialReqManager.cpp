#include <iostream>
#include "SerialReqManager.h"
#include <AmjuSleep.h>

//#define NO_THREADS

namespace Amju
{
SerialReqManager::SerialReqManager()
{
  m_thread = 0;
}

#ifdef NO_THREADS
void SerialReqManager::Update()
{
}

bool SerialReqManager::AddReq(RCPtr<OnlineReq> req, int maxRequestsOfThisType)
{
std::cout << "REQ: " << req->GetName() << "\n";

  req->DoRequest(m_hc);   
  req->HandleResult();
  return true;
}
#else

void SerialReqManager::SerialThread::Work()
{
  while (!m_stop)
  {
    m_mutex.Lock();
    RCPtr<OnlineReq> r = m_req;
    bool ready = (r && !r->IsFinished());
    m_mutex.Unlock();

    if (ready)
    {
      r->DoRequest(m_hc);
    
      m_mutex.Lock();
      m_req = 0;
      m_mutex.Unlock();
    }
    else
    {
      SleepMs(10); 
    }
  }
}

void SerialReqManager::SerialThread::SetReq(RCPtr<OnlineReq> req)
{
  MutexLocker mlock(m_mutex);

  Assert(m_req == 0);
  m_req = req;
}

bool SerialReqManager::SerialThread::Waiting()  const
{
  MutexLocker mlock(m_mutex);

  return (m_req == 0);
}

void SerialReqManager::Update()
{
  if (m_reqs.empty())
  {
    return;
  }

  OnlineReq* req = m_reqs.front();

  if (req->IsFinished())
  {
    req->HandleResult();
    m_reqs.pop_front();
  }

  if (m_reqs.empty())
  {
    return;
  }

  req = m_reqs.front();

  if (m_thread->Waiting())
  {
    m_thread->SetReq(req);
  }
}

bool SerialReqManager::AddReq(RCPtr<OnlineReq> req, int maxRequestsOfThisType)
{
  // If there are more than maxRequestsOfThisType already in the queue,
  //  remove the earlier ones.
  // This lets us get rid of old positions which will be overwritten, etc.
  // TODO

//std::cout << "QUEUE REQ: " << req->GetName() << "\n";

  m_reqs.push_back(req);

  if (!m_thread)
  {
    m_thread = new SerialThread;
    m_thread->Start();
  }

  return true;
}
#endif

int SerialReqManager::CountAllReqs() const
{
  return m_reqs.size();
}
}
