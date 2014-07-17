#ifndef MESSAGE_QUEUE_H_INCLUDED
#define MESSAGE_QUEUE_H_INCLUDED

#include <set>
#include <Singleton.h>
#include <RCPtr.h>
#include <AmjuTime.h>
#include <Mutex.h>

namespace Amju
{
// Time for Messages: may be adjusted to compensate for server lag etc.
typedef Time MsgTime;

struct Message : public RefCounted
{
  virtual ~Message() {}
  virtual void Execute() = 0;

  // Timestamp: the time at which this Message should be Executed
  MsgTime m_time;
};

typedef RCPtr<Message> PMessage;

inline bool operator<(const PMessage& m1, const PMessage& m2)
{
  return (m1->m_time < m2->m_time);
}


// Priority queue of messages. 
// Messages are executed when the current time reaches the
//  timestamp for a message, allowing for timed messages.
class MessageQueue : public NonCopyable
{
public:
  // Call from main thread once per frame
  void Update();

  // Can be called from main thread or other threads
  void Add(PMessage); 

  // Call to set the game time - can be seconds since server started, 
  //  just needs to be the same across all hosts.
  void SetTime(float seconds);

protected:
  // Priority queue: could be implemented as a Heap, Set, std::priority_queue...
  typedef std::set<PMessage> Queue;
  Queue m_q;
  
  // Current time
  float m_time;

  // Thread safe: Add() can be called from other threads. 
  Mutex m_mutex;
};

typedef Singleton<MessageQueue> TheMessageQueue;
}

#endif

