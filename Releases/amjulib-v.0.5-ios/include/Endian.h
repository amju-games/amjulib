/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Endian.h,v $
Revision 1.3  2007/09/22 20:25:07  jay
Move types to AmjuTypes.h, first step towards decoupling from OpenGL

Revision 1.2  2006/01/16 13:34:19  jay
Add test for Mac Intel

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ENDIAN_H_INCLUDED)
#define SCHMICKEN_ENDIAN_H_INCLUDED

#include "AmjuTypes.h"

// These functions swap the order of bytes. This would be useful when a 
// binary file saved under one Endianness was loaded under the other.
namespace Amju
{
int16 Endian(int16 i);
uint16 Endian(uint16 u);
int32 Endian(int32 i);
uint32 Endian(uint32 u);
float32 Endian(float32 f);
}
#endif
