/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Common.h,v $
Revision 1.6  2007/12/12 16:50:37  staff
Windows: VC8: resolve fabs ambiguity

Revision 1.5  2007/11/24 22:31:42  jay
Fix include path

Revision 1.4  2007/11/13 21:33:49  jay
Remove OpenGL calls

Revision 1.3  2007/11/08 00:26:38  jay
First pass at wrapping all OpenGL calls

Revision 1.2  2007/09/22 20:25:08  jay
Move types to AmjuTypes.h, first step towards decoupling from OpenGL

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#if !defined(COMMON_H_INCLUDED)
#define COMMON_H_INCLUDED

#include <math.h>

#ifdef WIN32
// VC8: fabs is ambiguous
#define fabs fabsf
#endif

namespace Amju
{
const double pi =  3.14159265359;
const float SMALLEST = 0.00005f;
}

#include "AmjuTypes.h"

#endif // COMMON_H_INCLUDED



