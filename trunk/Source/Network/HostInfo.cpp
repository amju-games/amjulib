#include "HostInfo.h"

#ifdef GEKKO
#include <network.h>

u32 net_gethostip(); // http://forum.wiibrew.org/read.php?11,7676,7678

#endif

namespace Amju
{
bool GetHostInfo(InfoVec* v)
{
#ifdef GEKKO
  // TODO Check valid, return false if network not initialised

  IfInfo ifi;
  //unsigned char m_mac[6];
  //unsigned char m_ip[4];
  //unsigned char m_ipmask[4];

  net_get_mac_address(ifi.m_mac);
  // Yuck, TODO 
  *(int*)(ifi.m_ip) = net_gethostip();
  // TODO mask

  v->push_back(ifi);
  return true;
#endif

}
}

