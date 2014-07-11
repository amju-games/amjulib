#include "ServSockThread.h"

namespace tsServer
{
  void ServSockThread::Work()
  {
    while (true)
    {
      Socket sock = servSocket.Accept();
 
      std::cout << "Whoa, got a connection!\n";
      SockThread* st = new SockThread(sock);
      st->Start();
    }
  }
}

