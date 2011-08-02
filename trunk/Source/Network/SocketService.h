/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SocketService.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

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
