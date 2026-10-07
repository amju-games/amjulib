// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "ServerSocket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef GEKKO
#include <network.h>
#endif
#include <AmjuFinal.h>

#ifdef ANDROID_NDK
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/endian.h>
#endif

namespace Amju
{
ServerSocket::ServerSocket(int port)
{
  AMJU_CALL_STACK;

  Bind(port);

#ifdef GEKKO
  net_listen(m_socket, 3);
#else
  listen(m_socket, 3);
#endif
}

Socket ServerSocket::Accept()
{
  AMJU_CALL_STACK;

#ifdef GEKKO
  return net_accept(m_socket, 0, 0);
#else
  return accept(m_socket, 0, 0);
#endif
}
}

