// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef __gluint_h__
#define __gluint_h__

extern const unsigned char *__gluNURBSErrorString( int errnum );

extern const unsigned char *__gluTessErrorString( int errnum );

#ifdef _EXTENSIONS_
#define COS cosf
#define SIN sinf
#define SQRT sqrtf
#else
#define COS cos
#define SIN sin
#define SQRT sqrt
#endif

#endif /* __gluint_h__ */
