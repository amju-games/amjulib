// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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

