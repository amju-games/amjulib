#ifndef MESSAGE_QUEUE_H_INCLUDED
#define MESSAGE_QUEUE_H_INCLUDED

#include <set>
#include <Singleton.h>
#include <RCPtr.h>
#include <AmjuTime.h>

namespace Amju
{
// Time for Messages: may be adjusted to compensate for server lag etc.
class MsgTime : public Time
{
};

struct Message : public RefCounted
{
  virtual ~Message() {}
  virtual void Execute() = 0;

  // Timestamp: the time at which this Message should be Executed
  MsgTime m_time;
};

typedef RCPtr<Message> PMessage;

bool operator<(const PMessage& m1, const PMessage& m2)
{
  return (m1->m_time < m2->m_time);
}


// Priority queue of messages. Messages are executed when the current time reaches the
//  timestamp for a message, allowing for timed messages.
// TODO MT-safe version or flag
class MessageQueue : public NonCopyable
{
public:
  void Update();
  void Add(PMessage); 

protected:
  // Priority queue: could be implemented as a Heap, Set, std::priority_queue...
  typedef std::set<PMessage> Queue;
  Queue m_q;
  
  // Current time
  MsgTime m_time;
};
}

#endif

