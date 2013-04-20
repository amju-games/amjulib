#import <Foundation/NSBundle.h>
#include "iOSGetDir.h"
#include <StringUtils.h>

std::string iOSGetDataDir()
{
  // Get path to data files
  NSString *filePath = [[NSBundle mainBundle] pathForResource:@"Info" ofType:@"plist"];

  // From http://forums.macrumors.com/showthread.php?t=494103
  //NSString *foo = @"your text here";
  //const char *bar = [foo UTF8String];

  const char* cFilePath = [filePath UTF8String];
  return Amju::GetFilePath(cFilePath) + "/Contents/Resources/";
}

std::string iOSGetSaveDir()
{
  return ""; // TODO
}


