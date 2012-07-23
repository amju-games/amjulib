/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SocketService.cpp,v $
Revision 1.3  2008/05/18 16:04:31  jay
MSVC update

Revision 1.2  2008/05/08 10:58:06  jay
New memory management code

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#include "SocketService.h"

#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef GEKKO
#include <network.h>
#include <errno.h>
#endif

#include "AmjuFinal.h"

namespace Amju
{
SocketService::SocketService() 
{ 
  AMJU_CALL_STACK;

#if defined(WIN32)
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

#ifdef GEKKO
  while (net_init() == -EAGAIN)
  {
    // TODO Yield
  }
#endif
}

SocketService::~SocketService()
{
  AMJU_CALL_STACK;

#if defined(WIN32)
  //WSACleanup();
#endif

// TODO GEKKO cleanup ?
}
}
