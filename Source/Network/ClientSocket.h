// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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

