#include "iOSGetUrl.h"
#include "StringUtils.h"
#import <Foundation/NSString.h>

bool iOSGetUrl(const std::string& url, std::string* result)
{
  // Convert std::string to NSString
  NSString* converted = [NSString stringWithUTF8String:url.c_str()];
  
  NSError* myError;
  NSString *pageContents = [NSString stringWithContentsOfURL:[NSURL URLWithString: converted] encoding:NSUTF8StringEncoding error:&myError];
    
  if (!pageContents)
  {
    *result = [myError.domain UTF8String];
    *result += " (code " + Amju::ToString((int)[myError code]) + ")";
    return false;
  }
  const char* res = [pageContents UTF8String];
  *result = std::string(res);
  return true;
}


