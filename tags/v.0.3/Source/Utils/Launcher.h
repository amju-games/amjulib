/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Launcher.h,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(AMJU_UTILS_LAUNCHER_H_INCLUDED)
#define AMJU_UTILS_LAUNCHER_H_INCLUDED

namespace Amju
{
bool LaunchProcess(const char* url);

bool LaunchURL(const char* url);
}

#endif

