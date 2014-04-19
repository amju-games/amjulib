/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ServerSocket.cpp,v $
Revision 1.3  2008/05/18 16:04:30  jay
MSVC update

Revision 1.2  2008/05/08 10:58:06  jay
New memory management code

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// Sockets code based on sample code in BSD Sockets Primer by Jim Frost
// http://world.std.com/~jimf/papers/sockets/sockets.html

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

