#pragma once

#include <map>
#include <string>
#include <Thread.h>
#include <Mutex.h>
#include <Singleton.h>

namespace Amju
{
// For processes that want to listen on a socket.
// Call the public member functions from the main thread.
// Thread-safe.
class Server
{
public:
  typedef std::map<int, std::string> Clients; // TODO ClientInfo

  bool Start(int port);
 
private:
  Mutex m_mutex;
  Clients m_clients;

};

typedef Singleton<Server> TheServer;
}

