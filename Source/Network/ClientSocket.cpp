/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientSocket.cpp,v $
Revision 1.2  2008/05/08 10:58:03  jay
New memory management code

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

// Sockets code based on sample code in BSD Sockets Primer by Jim Frost
// http://world.std.com/~jimf/papers/sockets/sockets.html


#include <AmjuFirst.h>
#include "ClientSocket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef GEKKO
#include <network.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <AmjuFinal.h>


namespace Amju
{
ClientSocket::ClientSocket()
{
  AMJU_CALL_STACK;

}

bool ClientSocket::Connect(const std::string& serverName, int port)
{
  AMJU_CALL_STACK;

  struct sockaddr_in sa;
  struct hostent *hp;

#ifdef GEKKO
  hp = net_gethostbyname(serverName.c_str());
#else
  hp = gethostbyname(serverName.c_str());
#endif

  if (!hp) 
  { 
    return false;                                
  }

  memset(&sa,0,sizeof(sa));
  memcpy((char *)&sa.sin_addr,hp->h_addr,hp->h_length);     
  sa.sin_family= hp->h_addrtype;
  sa.sin_port= htons((u_short)port);

#ifdef GEKKO
  if ((m_socket = net_socket(hp->h_addrtype,SOCK_STREAM,0)) < 0) 
#else
  if ((m_socket = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0) 
#endif
  {
    return false;
  }

#ifdef GEKKO
  if (net_connect(m_socket,(struct sockaddr *)&sa,sizeof sa) < 0) 
#else
  if (connect(m_socket,(struct sockaddr *)&sa,sizeof sa) < 0) 
#endif
  {
    return false;
  }
  return true;
}
}

