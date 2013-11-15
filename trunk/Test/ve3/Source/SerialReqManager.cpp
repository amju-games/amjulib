#include <AmjuFirst.h>
#include <iostream>
#include <AmjuSleep.h>
#include <Timer.h>
#include "SerialReqManager.h"
#include "ROConfig.h"
#include <AmjuFinal.h>

#define REUSE_HTTP_CLIENT

//#define SRM_AUDIT
//#define QUEUE_DEBUG
//#define NO_THREADS
//#define SRM_DEBUG
//#define SRM_LOCK_DEBUG
//#define SRM_SLEEP_DEBUG
//#define KILL_HANGING_REQ

namespace Amju
{
SerialReqManager::SerialReqManager()
{
  m_thread = 0;
  m_numResponses = 0;
  m_numQueued = 0;
}

int SerialReqManager::GetTotalNumResponses() const
{
  return m_numResponses;
}

//int SerialReqManager::GetTotalNumQueued() const
//{
//  return m_numQueued;
//}
  
Strings SerialReqManager::GetRequestNames() const
{
  Strings strs;

  for (OnlineReqs::const_iterator it = m_reqs.begin(); it != m_reqs.end(); ++it)
  {
    strs.push_back((*it)->GetName());
  }

  return strs;
}

void SerialReqManager::Clear()
{
  if (!m_reqs.empty())
  {
    m_reqs.erase(m_reqs.begin() + 1, m_reqs.end());
  }
}

#ifdef NO_THREADS
void SerialReqManager::Update()
{
}

bool SerialReqManager::AddReq(RCPtr<OnlineReq> req, int maxRequestsOfThisType, bool)
{
std::cout << "REQ: " << req->GetName() << "\n";

  static HttpClient hc;
  req->DoRequest(hc);   
  req->HandleResult();
  return true;
}
#else

void SerialReqManager::SerialThread::Work()
{
  while (!m_stop)
  {
    m_mutex.Lock();

#ifdef SRM_LOCK_DEBUG
std::cout << "SRM: Locked1\n";
#endif

    RCPtr<OnlineReq> r = m_req;
    bool ready = (r && !r->IsFinished());
    m_mutex.Unlock();

#ifdef SRM_LOCK_DEBUG
std::cout << "SRM: Unlocked1\n";
#endif

    if (ready)
    {
#ifdef SRM_DEBUG
std::cout << "SRM: Doing request: " << r->GetName() << "\n";
#endif

#ifdef REUSE_HTTP_CLIENT
      r->DoRequest(m_hc);
#else
      HttpClient hc;
      r->DoRequest(hc);
#endif

#ifdef SRM_DEBUG
std::cout << "SRM: Done request: " << r->GetName() << "\n";
#endif
    
      m_mutex.Lock();
#ifdef SRM_LOCK_DEBUG
std::cout << "SRM: Locked2\n";
#endif

      m_req = 0;
      m_mutex.Unlock();

#ifdef SRM_LOCK_DEBUG
std::cout << "SRM: Unlocked2\n";
#endif
    }
    else
    {
#ifdef SRM_SLEEP_DEBUG
std::cout << "SRM: Sleep\n";
#endif

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

  if (!m_thread)
  {
    return;
  }

  RCPtr<OnlineReq> req = m_reqs.front();

  if (m_thread->Waiting() && !req->IsFinished())
  {
#ifdef SRM_AUDIT
std::cout << "SRM: Set thread request: " << req->GetName() << "\n";
#endif

    m_thread->SetReq(req);
    return;
  }

  static float reqTime = 0;
  if (req->IsFinished())
  {
#ifdef SRM_AUDIT
std::cout << "Request " << req->GetName() << " took " << reqTime << "s to complete.\n";
#endif

    reqTime = 0;
    req->HandleResult();

    m_numResponses++;

#ifdef SRM_AUDIT
    std::cout << "SRM: Total responses: " << m_numResponses << "\n";
#endif

    m_reqs.pop_front();
  }
  else
  {
    static const float HANG_TIME = ROConfig()->GetFloat("hang-time", 10.0f);

    float dt = TheTimer::Instance()->GetDt();
    reqTime += dt;
    if (reqTime > HANG_TIME) // Hanging, on Mac this is a problem
    {
std::cout << "Reqest " << req->GetName() << " may have hung, time is " << reqTime << "s.\n";

#ifdef KILL_HANGING_REQ
      ////m_reqs.pop_front();
      // The thread is now blocked. 
      // TODO if there was a way of killing the request, that would be best
      m_thread = new SerialThread;
      m_thread->Start();
      reqTime = 0;
#endif

    }
  }
}

bool SerialReqManager::AddReq(RCPtr<OnlineReq> req, int maxRequestsOfThisType, bool discardNewNotOld)
{
  // If there are more than maxRequestsOfThisType already in the queue,
  //  remove the earlier ones.
  // This lets us get rid of old positions which will be overwritten, etc.
  // TODO

#ifdef SRM_AUDIT
std::cout << "SRM: Queueing request: \"" << req->GetName() << "\"\n";
#endif

  int count = CountReqsWithName(req->GetName());
  if (count >= maxRequestsOfThisType)
  {
    // Discard this msg, or older msgs, depending on flag.
    if (discardNewNotOld)
    {
      // Easy - dump this msg :-)
#ifdef QUEUE_DEBUG
std::cout << "QUEUE REQ: DISCARDING NEWEST: " << req->GetName() << "\n";
#endif

      return false;
    }
    else
    {
#ifdef QUEUE_DEBUG
std::cout << "QUEUE REQ: DISCARDING OLDEST: " << req->GetName() << " found: " << count 
  << ", max: " << maxRequestsOfThisType << "\n";
#endif

      // Check current req - is it the same type ? If so, reduce count as we can't remove this one.
      if (m_reqs[0]->GetName() == req->GetName())
      {
        count--;
      }

      // Iterate over queued requests, oldest first
      while (count >= maxRequestsOfThisType)
      {
        bool found = false;
        for (OnlineReqs::iterator it = m_reqs.begin() + 1; // start after current.
             it != m_reqs.end();
             ++it)
        {
          if ((*it)->GetName() == req->GetName())
          {
            found = true;
            m_reqs.erase(it);
            count--;
            break; // can't keep iterating, invalidated
          }
        }
  
        if (!found)
        {
std::cout << "Logic error, count is " << count << " but no reqs of type " << req->GetName() << " found.\n";
          Assert(0);
        }
      }
    }
  }

  m_reqs.push_back(req);

  m_numQueued++;

#ifdef SRM_AUDIT
std::cout << "SRM: Total ever queued: " << m_numQueued << " elapsed: " 
  << TheTimer::Instance()->GetElapsedTime() << "\n";
#endif

#ifdef QUEUE_DEBUG
std::cout << "Here is the queue now: \n";
for (unsigned int i = 0; i < m_reqs.size(); i++)
{
  std::string url = m_reqs[i]->GetUrl();
  const unsigned int MAX_URL_LEN = 60;
  if (url.size() > MAX_URL_LEN)
  {
    url = url.substr(0, MAX_URL_LEN);
  }
  std::cout << i << ": " << m_reqs[i]->GetName() << "\t" << url << "\n";
}
#endif

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

int SerialReqManager::CountReqsWithName(const std::string& name)
{
  int count = 0;
  for (OnlineReqs::iterator it = m_reqs.begin(); it != m_reqs.end(); ++it)
  {
    if ((*it)->GetName() == name)
    {
      ++count;
    }
  }
  return count;
}

}
