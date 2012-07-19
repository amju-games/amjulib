/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientSocket.h,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#if !defined(AMJU_CLIENT_SOCKET_H_INCLUDED)
#define AMJU_CLIENT_SOCKET_H_INCLUDED

#include "Socket.h"

namespace Amju
{
class ClientSocket : public Socket
{
public:
  ClientSocket();

  // Connect to the specified server on the port given.
  // Returns true if connection succeeds.
  bool Connect(const std::string& serverName, int port);
};
}
#endif

