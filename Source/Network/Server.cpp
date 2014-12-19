#include <iostream>
#include <Locker.h>
#include "ServerSocket.h"
#include "Server.h"

namespace Amju
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

class ServSockThread : public Thread
{
public:
  ServSockThread(int port) : m_serv(port) {}

  void Work() //override
  {
    int id = 0; // each client gets unique ID
    while (true)
    {
      Socket sock = m_serv.Accept();
      id++;

      std::cout << "Whoa, got a connection!\n";
      SockThread* st = new SockThread(id, sock);
      st->Start();
    }
  }

private:
  ServerSocket m_serv;
};

bool Server::Start(int port)
{
  // TODO failure conditions
  ServSockThread* thr = new ServSockThread(port);
  thr->Start();
  std::cout << "Started new serv sock thread..\n";
  return true;
}
 
}

