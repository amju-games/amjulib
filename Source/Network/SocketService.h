// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(AMJU_SOCKET_SERVICE_H_INCLUDED)
#define AMJU_SOCKET_SERVICE_H_INCLUDED

#include <Singleton.h>

namespace Amju
{
class SocketService
{
  SocketService();
  friend class Singleton<SocketService>;
public:
  ~SocketService();
};

typedef Singleton<SocketService> TheSocketService;
}
#endif
