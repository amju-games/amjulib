#pragma once

#include <iostream>
#include <string>
#include "Thread.h"
#include "ServerSocket.h"
#include "SockThread.h"
#include "Server.h"

namespace tsServer
{
class ServSockThread : public Thread
{
public:
  ServSockThread(int port) : m_serv(port) {}

  void Work() //override
  {
    int id = 0;
    while (true)
    {
      Socket sock = m_serv.Accept();
 
      std::cout << "Whoa, got a connection!\n";
      SockThread* st = new SockThread(id, sock);
      st->Start();

      id++;
      // Remote host name?
      std::string name = "TODO";
      theServer.addClient(st);
    }
  }

private:
  ServerSocket m_serv;
};
}

