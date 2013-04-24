#include "iOSGetUrl.h"
#import <Foundation/NSString.h>

bool iOSGetUrl(const std::string& url, std::string* result)
{
  // Convert std::string to NSString
  NSString* converted = [NSString stringWithUTF8String:url.c_str()];
    
  NSString *pageContents = [NSString stringWithContentsOfURL:[NSURL URLWithString: converted] encoding:NSUTF8StringEncoding error:nil];
    
  if (!pageContents)
  {
    // TODO Set error string
    return false;
  }
  const char* res = [pageContents UTF8String];
  *result = std::string(res);
  return true;
}


