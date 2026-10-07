// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "Launcher.h"
#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#ifdef MACOSX
#include <unistd.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFUrl.h>
#include <Carbon/Carbon.h>
#endif

#ifdef AMJU_IOS
#include <unistd.h>
#endif

#include <AmjuFinal.h>

namespace Amju
{
bool LaunchProcess(const char* url)
{
#if defined(MACOSX) || defined(IPHONE)
  int ret = execv(url, 0);  // TODO args
  return ret != -1;
#endif

#ifdef WIN32
  return LaunchURL(url);
#endif

 // TODO GEKKO
  return false;
}

bool LaunchURL(const char* url)
{
  AMJU_CALL_STACK;

#ifdef WIN32
  auto r = (ShellExecuteA(0, "open", url, "", "", 1));
  return (reinterpret_cast<int>(r) > 32);

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

