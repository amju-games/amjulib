// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_loadso_h
#define _SDL_loadso_h

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This function dynamically loads a shared object and returns a pointer
 * to the object handle (or NULL if there was an error).
 * The 'sofile' parameter is a system dependent name of the object file.
 */
extern DECLSPEC void * SDLCALL SDL_LoadObject(const char *sofile);

/* Given an object handle, this function looks up the address of the
 * named function in the shared object and returns it.  This address
 * is no longer valid after calling SDL_UnloadObject().
 */
extern DECLSPEC void * SDLCALL SDL_LoadFunction(void *handle, const char *name);

/* Unload a shared object from memory */
extern DECLSPEC void SDLCALL SDL_UnloadObject(void *handle);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_loadso_h */
