/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#ifndef AMJU_ASSERT_H_INCLUDED
#define AMJU_ASSERT_H_INCLUDED

#if defined(WIN32)

#include <iostream>

// This is to highlight uses of built-in assert, which we don't want.
#if defined(assert)
#undef assert
#define assert(exp) ?????
#endif // assert

#undef Assert

#if defined(_DEBUG)
namespace Amju
{
  // We use a custom version of assert. 
  // The reason is that the standard MSVC assert tries to pop up a dialog.
  // If the assert occurs during drawing, which it practically always does,
  // it appears that trying to display the dialog causes a redraw, so the 
  // assert occurs again... so the stack overflows and you don't get any
  // information at all.
  // This version avoids the problem by writing to the console, so you need
  // the GLUT version of the executable.

  void winAssert(const void* b, const void*, unsigned);
  
}
// NB We use upper-case 'A' to avoid accidentally using the built-in 
// assert - it's very easy to do!
#define Assert(exp) (void)( (exp) || (Amju::winAssert(#exp, __FILE__, __LINE__), 0) )
#else
#define Assert(exp) ((void)0)
#endif // _DEBUG
#endif // WIN32

#if defined(MACOSX) || defined(IPHONE) 
#include <assert.h>
#define Assert assert
#endif // MACOSX or IPHONE

#ifdef GEKKO

namespace Amju
{
  void wiiAssert(const void* exp, const char* file, unsigned int line);
}  
#define Assert(exp) (void)( (exp) || (Amju::wiiAssert(#exp, __FILE__, __LINE__), 0) )
#endif //  GEKKO

#ifdef ANDROID_NDK
#include <assert.h>
#define Assert assert
#endif

#endif // include guard

