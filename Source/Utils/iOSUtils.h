#ifndef AMJU_IOS_UTILS_H_INCLUDED
#define AMJU_IOS_UTILS_H_INCLUDED

#ifdef AMJU_IOS

#include <string>

namespace Amju
{
std::string GetDevicePreferredLanguage();
  
enum iOSDeviceType { AMJU_IPAD, AMJU_IPHONE };
iOSDeviceType GetDeviceType();

void GetDeviceInfo(int* deviceId, std::string* deviceUserName, std::string* model, std::string* osVersion);
  
/*
 model strings, from http://stackoverflow.com/questions/11197509/ios-how-to-get-device-make-and-model
 @"i386"      on 32-bit Simulator
 @"x86_64"    on 64-bit Simulator
 @"iPod1,1"   on iPod Touch
 @"iPod2,1"   on iPod Touch Second Generation
 @"iPod3,1"   on iPod Touch Third Generation
 @"iPod4,1"   on iPod Touch Fourth Generation
 @"iPod7,1"   on iPod Touch 6th Generation
 @"iPhone1,1" on iPhone
 @"iPhone1,2" on iPhone 3G
 @"iPhone2,1" on iPhone 3GS
 @"iPad1,1"   on iPad
 @"iPad2,1"   on iPad 2
 @"iPad3,1"   on 3rd Generation iPad
 @"iPhone3,1" on iPhone 4 (GSM)
 @"iPhone3,3" on iPhone 4 (CDMA/Verizon/Sprint)
 @"iPhone4,1" on iPhone 4S
 @"iPhone5,1" on iPhone 5 (model A1428, AT&T/Canada)
 @"iPhone5,2" on iPhone 5 (model A1429, everything else)
 @"iPad3,4" on 4th Generation iPad
 @"iPad2,5" on iPad Mini
 @"iPhone5,3" on iPhone 5c (model A1456, A1532 | GSM)
 @"iPhone5,4" on iPhone 5c (model A1507, A1516, A1526 (China), A1529 | Global)
 @"iPhone6,1" on iPhone 5s (model A1433, A1533 | GSM)
 @"iPhone6,2" on iPhone 5s (model A1457, A1518, A1528 (China), A1530 | Global)
 @"iPad4,1" on 5th Generation iPad (iPad Air) - Wifi
 @"iPad4,2" on 5th Generation iPad (iPad Air) - Cellular
 @"iPad4,4" on 2nd Generation iPad Mini - Wifi
 @"iPad4,5" on 2nd Generation iPad Mini - Cellular
 @"iPad4,7" on 3rd Generation iPad Mini - Wifi (model A1599)
 @"iPhone7,1" on iPhone 6 Plus
 @"iPhone7,2" on iPhone 6
 @"iPhone8,1" on iPhone 6S
 @"iPhone8,2" on iPhone 6S Plus
 @"iPhone8,4" on iPhone SE
*/
  
}

#endif

#endif

