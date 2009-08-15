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

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include "ServerSocket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif
#include "AmjuFinal.h"

namespace Amju
{
ServerSocket::ServerSocket(int port)
{
  AMJU_CALL_STACK;

  Bind(port);
  listen(m_socket, 3);
}

Socket ServerSocket::Accept()
{
  AMJU_CALL_STACK;

  return accept(m_socket, 0, 0);
}
}

