/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Launcher.cpp,v $
Revision 1.4  2008/05/18 16:04:27  jay
MSVC update

Revision 1.3  2008/05/08 10:58:05  jay
New memory management code

Revision 1.2  2007/03/29 18:46:18  jay
Watch out for non-english encoding

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#include "Launcher.h"
#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif
#ifdef MACOSX
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFUrl.h>
#include <Carbon/Carbon.h>
#endif
#include "AmjuFinal.h"

namespace Amju
{
bool Launch(const char* url)
{
  AMJU_CALL_STACK;

#ifdef WIN32
  int r =  (int)ShellExecute(0, "open", url, "", "", 1);
  return (r > 32);

#else 

#ifdef MACOSX

  CFStringRef strurl =                //CFSTR("http://www.amju.com"); // TODO HACK 
    CFStringCreateWithBytes(
    0, 
    (const unsigned char*)url, 
    strlen(url),
    kCFStringEncodingMacRoman,
    false);

  CFURLRef cfurl = CFURLCreateWithString(0, strurl, 0);
  OSStatus ret = LSOpenCFURLRef(cfurl, 0);

/*
// TODO This could fail on non-english encoding
std::cout << "Launch URL: " << CFStringGetCStringPtr(strurl, kCFStringEncodingMacRoman) << "\n";
*/

std::cout << "Launch result: " << GetMacOSStatusErrorString(ret) << "\n";

  // TODO deal with ret
  return true;

#else
  // Not implemented for this platform
  return false;
#endif
#endif

}
}

