// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "SocketService.h"

#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef GEKKO
#include <network.h>
#include <errno.h>
#include <AmjuFinal.h>
#endif


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
