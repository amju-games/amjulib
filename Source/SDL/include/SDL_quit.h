// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_quit_h
#define _SDL_quit_h

#include "SDL_stdinc.h"
#include "SDL_error.h"

/* 
  An SDL_QUITEVENT is generated when the user tries to close the application
  window.  If it is ignored or filtered out, the window will remain open.
  If it is not ignored or filtered, it is queued normally and the window
  is allowed to close.  When the window is closed, screen updates will 
  complete, but have no effect.

  SDL_Init() installs signal handlers for SIGINT (keyboard interrupt)
  and SIGTERM (system termination request), if handlers do not already
  exist, that generate SDL_QUITEVENT events as well.  There is no way
  to determine the cause of an SDL_QUITEVENT, but setting a signal
  handler in your application will override the default generation of
  quit events for that signal.
*/

/* There are no functions directly affecting the quit event */
#define SDL_QuitRequested() \
        (SDL_PumpEvents(), SDL_PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUITMASK))

#endif /* _SDL_quit_h */
