#ifdef AMJU_IOS

#include <sys/time.h>
#import <sys/utsname.h> // import it in your header or implementation file.
#import <UIKit/UIKit.h>
#include "AmjuHash.h"
#include "iOSUtils.h"

namespace Amju
{
std::string GetDevicePreferredLanguage()
{
  // https://stackoverflow.com/questions/3910244/getting-current-device-language-in-ios
  
  NSString *language = [[NSLocale preferredLanguages] objectAtIndex:0];
  std::string s([language UTF8String]);
  return s;
}
  
iOSDeviceType GetDeviceType()
{
  static iOSDeviceType t = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) ?
    AMJU_IPAD : AMJU_IPHONE;

  return t;
}
  
void GetDeviceInfo(int* deviceId, std::string* deviceUserName, std::string* model, std::string* osVersion)
{
  if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)])
  {
    NSString* deviceIdStr = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    *deviceId = static_cast<int>(HashString([deviceIdStr UTF8String]));
  }
  else
  {
    // Get current time usecs
    struct timeval time;
    gettimeofday(&time, NULL);
    *deviceId = static_cast<int>(time.tv_usec);
  }

  *deviceUserName = [[[UIDevice currentDevice] name] UTF8String];
  
  // Better, more detailed model name
  //  http://stackoverflow.com/questions/11197509/ios-how-to-get-device-make-and-model
  struct utsname systemInfo;
  uname(&systemInfo);
  NSString* betterModelName = [NSString stringWithCString:systemInfo.machine
                              encoding:NSUTF8StringEncoding];
  *model = [betterModelName UTF8String];

  *osVersion = [[[UIDevice currentDevice] systemVersion] UTF8String];
}
}

#endif

