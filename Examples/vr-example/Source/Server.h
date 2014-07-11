#pragma once

#include <map>
#include <string>
#include "Mutex.h"

namespace tsServer
{
class SockThread;

// Usage: one client connects. If another connects, it evicts the previous
// client.
// All public member functions are designed to be called from the main
// thread and are thread-safe.
class CServer
{
public:
  CServer();

  bool start(int port);
  
///  std::string getClientName();

  // Returns true the first time called after new connection
  bool isNewClient();

  // Returns true if there is string data waiting
  bool hasData();

  // Returns string recvd from client
  // Returns immediately
  std::string recv();

  // Send string to client 
  bool send(const std::string& str);

private:
  friend class SockThread;
  friend class ServSockThread;
  void addClient(SockThread* st);
  
  Mutex mutex;

  SockThread* sockThread;
  bool isNew;
};

extern CServer theServer;
}

