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

}

#endif

