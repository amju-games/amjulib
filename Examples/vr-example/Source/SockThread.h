#pragma once

#include "Server.h"
#include "Mutex.h"
#include "Locker.h"

namespace tsServer
{
class SockThread : public Thread
{
public:
  SockThread(int id, const Socket& s) : m_id(id), m_sock(s) {}
 
  void Work() // override
  {
    std::string str;
    while (!m_stop && m_sock.GetRawString(&str))
    {
      std::cout << "WOW! Got this string: " << str << "\n";

      MutexLocker lock(mutex);
      recvString += str;
    }
    std::cout << "This thread has been stopped.\n";
    m_sock.WriteRawString("Evicted!\n");
    m_sock.Close();
  }

  virtual void Finish()
  {
std::cout << "This thread is finished.\n"; // TODO name
    delete this;
  }

  bool hasData()
  {
    MutexLocker lock(mutex);
    return !recvString.empty();
  }
 
  bool send(const std::string& str)
  {
    return m_sock.WriteRawString(str);
  }

  std::string recv()
  {
    MutexLocker lock(mutex);
    std::string res = recvString;
    recvString.clear();
    return res; 
  }

public:
  Mutex mutex;
  int m_id;
  Socket m_sock;
  std::string recvString;

};
}

