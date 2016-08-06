#ifdef AMJU_IOS
#import <UIKit/UIKit.h>
#include "iOSUtils.h"

namespace Amju
{
iOSDeviceType GetDeviceType()
{
  static iOSDeviceType t = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) ?
    AMJU_IPAD : AMJU_IPHONE;

  return t;
}

void GetDeviceInfo(std::string* deviceId, std::string* deviceUserName, std::string* model, std::string* osVersion)
{
  if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)])
  {
    *deviceId = [[[[UIDevice currentDevice] identifierForVendor] UUIDString] UTF8String];
  }

  *deviceUserName = [[[UIDevice currentDevice] name] UTF8String];
  
  *model = [[[UIDevice currentDevice] model] UTF8String];

  *osVersion = [[[UIDevice currentDevice] systemVersion] UTF8String];
}
}

#endif

