// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_cpuinfo_h
#define _SDL_cpuinfo_h

#include "SDL_stdinc.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This function returns true if the CPU has the RDTSC instruction
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasRDTSC(void);

/* This function returns true if the CPU has MMX features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasMMX(void);

/* This function returns true if the CPU has MMX Ext. features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasMMXExt(void);

/* This function returns true if the CPU has 3DNow features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNow(void);

/* This function returns true if the CPU has 3DNow! Ext. features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNowExt(void);

/* This function returns true if the CPU has SSE features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE(void);

/* This function returns true if the CPU has SSE2 features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE2(void);

/* This function returns true if the CPU has AltiVec features
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasAltiVec(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_cpuinfo_h */
