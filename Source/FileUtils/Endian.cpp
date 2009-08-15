/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Endian.cpp,v $
Revision 1.4  2008/05/18 16:04:25  jay
MSVC update

Revision 1.3  2008/05/08 10:58:03  jay
New memory management code

Revision 1.2  2006/01/16 13:34:19  jay
Add test for Mac Intel

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include <algorithm> // swap
#include "Endian.h"
#include "AmjuFinal.h"

namespace Amju
{
// If Mac on Intel, no swapping is required.
#if defined(WIN32) || defined(__i386__)
int16 Endian(int16 i) { return i; }
uint16 Endian(uint16 u) { return u; }
int32 Endian(int32 i) { return i; }
uint32 Endian(uint32 u) { return u; }
float32 Endian(float32 f) { return f; }
#else

int16 Endian(int16 i) 
{
  AMJU_CALL_STACK;

  return (int16)(Endian((uint16)(i)));
}

uint16 Endian(uint16 u) 
{
  AMJU_CALL_STACK;

  uint16 r1 = ((u & 0x00ff) << 8);
  uint16 r2 = ((u & 0xff00) >> 8); 
  
  uint16 r = r1 + r2;
  return r;
}

int32 Endian(int32 i) 
{
  AMJU_CALL_STACK;

  return (int32)(Endian((uint32)(i)));
}

uint32 Endian(uint32 u) 
{
  AMJU_CALL_STACK;

  return ((u & 0x000000ff) << 24) | 
         ((u & 0x0000ff00) << 8) | 
         ((u & 0x00ff0000) >> 8) |
         ((u & 0xff000000) >> 24);
}

float32 Endian(float32 f) 
{ 
  AMJU_CALL_STACK;

// TODO Maybe the Gekko code is preferable for all platforms

#ifdef GEKKO
  float g = f;
  unsigned char* c = (unsigned char*)&g;
  std::swap(c[0], c[3]);
  std::swap(c[1], c[2]);
  return g;
  
#else
  // This doesn't work for Gekko - warning about "type-punned pointer" ?!
  uint32 r = Endian(*((uint32*)&f));
  float32 rf = *((float32*)&r);
  return rf;
#endif 
}
#endif
}

