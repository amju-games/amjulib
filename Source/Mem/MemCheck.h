/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MemCheck.h,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// MSVC only: switch on leak-detection for debug builds.
// This is from Tip Of The Day on flipcode <www.flipcode.com>.
#if !defined(MEMCHECK_H_INCLUDED)
#define MEMCHECK_H_INCLUDED

#if defined(WIN32) && defined(_DEBUG)
#include <crtdbg.h>

// Report file/line no. of leak
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

static int memflag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
memflag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
_CrtSetDbgFlag(memflag); // Set flag to the new value

#endif
#endif


