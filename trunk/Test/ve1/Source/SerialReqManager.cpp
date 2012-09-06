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

void SerialReqManager::Clear()
{
  m_reqs.clear();
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

bool SerialReqManager::AddReq(RCPtr<OnlineReq> req, int maxRequestsOfThisType, bool discardNewNotOld)
{
  // If there are more than maxRequestsOfThisType already in the queue,
  //  remove the earlier ones.
  // This lets us get rid of old positions which will be overwritten, etc.
  // TODO

//std::cout << "QUEUE REQ: " << req->GetName() << "\n";

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
