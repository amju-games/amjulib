// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_config_h
#define _SDL_config_h

#include "SDL_platform.h"

/* Add any platform that doesn't build using the configure system */
#if defined(__DREAMCAST__)
#include "SDL_config_dreamcast.h"
#elif defined(__MACOS__)
#include "SDL_config_macos.h"
#elif defined(__MACOSX__)
#include "SDL_config_macosx.h"
#elif defined(__SYMBIAN32__)
#include "SDL_config_symbian.h"  /* must be before win32! */
#elif defined(__WIN32__)
#include "SDL_config_win32.h"
#elif defined(__OS2__)
#include "SDL_config_os2.h"
#else
#include "SDL_config_minimal.h"
#endif /* platform config */

#endif /* _SDL_config_h */
