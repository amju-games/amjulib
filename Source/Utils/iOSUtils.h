#ifndef AMJU_IOS_UTILS_H_INCLUDED
#define AMJU_IOS_UTILS_H_INCLUDED

#ifdef AMJU_IOS

#include <string>

namespace Amju
{
enum iOSDeviceType { AMJU_IPAD, AMJU_IPHONE };
iOSDeviceType GetDeviceType();

void GetDeviceInfo(int* deviceId, std::string* deviceUserName, std::string* model, std::string* osVersion);
}

#endif

#endif

