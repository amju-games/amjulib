#ifndef DEG_RAD_H
#define DEG_RAD_H

#ifdef WIN32
#define _USE_MATH_DEFINES // ffs
#endif // WIN32

#include <math.h>

// (already defined in libogc)
#ifndef DegToRad 
#define DegToRad(x) (float)((x)*(M_PI/180.0))
#endif

#ifndef RadToDeg
#define RadToDeg(x) (float)((x)*(180.0/M_PI))
#endif

#endif
