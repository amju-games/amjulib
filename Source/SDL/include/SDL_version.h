// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_version_h
#define _SDL_version_h

#include "SDL_stdinc.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
*/
#define SDL_MAJOR_VERSION	1
#define SDL_MINOR_VERSION	2
#define SDL_PATCHLEVEL		12

typedef struct SDL_version {
	Uint8 major;
	Uint8 minor;
	Uint8 patch;
} SDL_version;

/* This macro can be used to fill a version structure with the compile-time
 * version of the SDL library.
 */
#define SDL_VERSION(X)							\
{									\
	(X)->major = SDL_MAJOR_VERSION;					\
	(X)->minor = SDL_MINOR_VERSION;					\
	(X)->patch = SDL_PATCHLEVEL;					\
}

/* This macro turns the version numbers into a numeric value:
   (1,2,3) -> (1203)
   This assumes that there will never be more than 100 patchlevels
*/
#define SDL_VERSIONNUM(X, Y, Z)						\
	((X)*1000 + (Y)*100 + (Z))

/* This is the version number macro for the current SDL version */
#define SDL_COMPILEDVERSION \
	SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL)

/* This macro will evaluate to true if compiled with SDL at least X.Y.Z */
#define SDL_VERSION_ATLEAST(X, Y, Z) \
	(SDL_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

/* This function gets the version of the dynamically linked SDL library.
   it should NOT be used to fill a version structure, instead you should
   use the SDL_Version() macro.
 */
extern DECLSPEC const SDL_version * SDLCALL SDL_Linked_Version(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_version_h */
