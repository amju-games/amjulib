#ifndef AMJU_HOST_INFO_H
#define AMJU_HOST_INFO_H

#include <vector>

namespace Amju
{
// Info about one interface
struct IfInfo
{
  unsigned char m_mac[6];
  unsigned char m_ip[4];
  unsigned char m_ipmask[4];
};

typedef std::vector<IfInfo> InfoVec;

bool GetHostInfo(InfoVec*);
}

#endif

