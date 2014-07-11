#include <iostream>
#include "Server.h"
#include "ServSockThread.h"
#include "Locker.h"

namespace tsServer
{
CServer theServer;

CServer::CServer() : sockThread(0), isNew(false)
{
}

bool CServer::start(int port)
{
  ServSockThread* thr = new ServSockThread(port);
  thr->Start();
  std::cout << "Started new serv sock thread..\n";
  return true;
}

bool CServer::isNewClient()
{
  MutexLocker lock(mutex);
  bool b = isNew;
  isNew = false;
  return b;
}

bool CServer::hasData()
{
  MutexLocker lock(mutex);
  return sockThread->hasData();
}

void CServer::addClient(SockThread* st)
{
  MutexLocker lock(mutex);
  if (sockThread)
  {
    sockThread->stop();
  }

  sockThread = st;
  isNew = true;
}

//std::string CServer::getClientName()
//{
//  MutexLocker lock(mutex);
//  return sockThread->getName();
//}

std::string CServer::recv()
{
  MutexLocker lock(mutex);
  return sockThread->recv();
}

bool CServer::send(const std::string& str)
{
  MutexLocker lock(mutex);
  return sockThread->send(str);
}

}

