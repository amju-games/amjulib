#import <Foundation/NSBundle.h>
#include "iOSGetDir.h"
#include <StringUtils.h>

std::string iOSGetDataDir()
{
  NSString *bundlePath = [[NSBundle mainBundle] resourcePath];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  // Use a sub-enumerator to look inside all folders
  NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:bundlePath];
  
  NSLog(@"--- START BUNDLE SCAN ---");
  NSString *filePath;
  while ((filePath = [enumerator nextObject])) {
    NSLog(@"FILE FOUND: %@", filePath);
  }
  NSLog(@"--- END BUNDLE SCAN ---");
  
  // Get path to data files
  NSString *filePath2 = [[NSBundle mainBundle] pathForResource:@"Info" ofType:@"plist"];

  // Convert NSString to char* From http://forums.macrumors.com/showthread.php?t=494103
  //NSString *foo = @"your text here";
  //const char *bar = [foo UTF8String];

  const char* cFilePath = [filePath2 UTF8String];
  return Amju::GetFilePath(cFilePath) + "/";
}

std::string iOSGetSaveDir()
{
  // From http://stackoverflow.com/questions/6907381/what-is-the-documents-directory-nsdocumentdirectory
    
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;

  const char* cFilePath = [basePath UTF8String];
  return std::string(cFilePath) + "/";
}


