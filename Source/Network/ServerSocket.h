/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ServerSocket.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SERVER_SOCKET_H_INCLUDED)
#define SCHMICKEN_SERVER_SOCKET_H_INCLUDED

#include "Socket.h"

namespace Amju
{
class ServerSocket : public Socket
{
public:
  ServerSocket(int port);
  Socket Accept();
};
}
#endif

