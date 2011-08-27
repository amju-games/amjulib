#include "HostInfo.h"

#ifdef GEKKO
#include <network.h>
u32 net_gethostip(); // http://forum.wiibrew.org/read.php?11,7676,7678
#endif

#if defined (MACOSX) || defined (IPHONE)
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif // MACOSX/IPHONE

#include <StringUtils.h>
#include <AmjuAssert.h>

namespace Amju
{
std::string ToString(const IfInfo& ifi)
{
  std::string s = "Name: ";
  s += ifi.m_name;
  s += " IP Addr: ";
  s += ToString((int)ifi.m_ip[0]);
  s += ".";
  s += ToString((int)ifi.m_ip[1]);
  s += ".";
  s += ToString((int)ifi.m_ip[2]);
  s += ".";
  s += ToString((int)ifi.m_ip[3]);
  s += " Mac Addr: ";
  for (int i = 0; i < 6; i++)
  {
    s += ToHexString(ifi.m_mac[i]);
  }
  return s;
}

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
#endif // GEKKO

#if defined (MACOSX) || defined (IPHONE)
  // http://blog.zachwaugh.com/post/309927273/programmatically-retrieving-ip-address-of-iphone?d0594d68

  struct ifaddrs *interfaces = 0;

  // retrieve the current interfaces - returns 0 on success
  int success = getifaddrs(&interfaces);

  if (success == 0)
  {
    // Loop through linked list of interfaces
    struct ifaddrs *temp_addr = interfaces;
    while (temp_addr)
    {
      if (temp_addr->ifa_addr->sa_family == AF_INET)
      {
        IfInfo ifi;
        ifi.m_name = temp_addr->ifa_name;
        // ("en0" is the wifi connection on the iPhone)

        Assert(sizeof(in_addr) == 4);
        memcpy(&ifi.m_ip, &((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr, 4);

        v->push_back(ifi);
      }

      temp_addr = temp_addr->ifa_next;
    }
  }

  freeifaddrs(interfaces); // Free memory
  return (success == 0);
#endif // MACOSX/IPHONE

#ifdef WIN32
  return false; // TODO
#endif // WIN32
}
}

