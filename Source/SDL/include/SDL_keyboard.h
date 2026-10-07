// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef _SDL_keyboard_h
#define _SDL_keyboard_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_keysym.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Keysym structure
   - The scancode is hardware dependent, and should not be used by general
     applications.  If no hardware scancode is available, it will be 0.

   - The 'unicode' translated character is only available when character
     translation is enabled by the SDL_EnableUNICODE() API.  If non-zero,
     this is a UNICODE character corresponding to the keypress.  If the
     high 9 bits of the character are 0, then this maps to the equivalent
     ASCII character:
	char ch;
	if ( (keysym.unicode & 0xFF80) == 0 ) {
		ch = keysym.unicode & 0x7F;
	} else {
		An international character..
	}
 */
typedef struct SDL_keysym {
	Uint8 scancode;			/* hardware specific scancode */
	SDLKey sym;			/* SDL virtual keysym */
	SDLMod mod;			/* current key modifiers */
	Uint16 unicode;			/* translated character */
} SDL_keysym;

/* This is the mask which refers to all hotkey bindings */
#define SDL_ALL_HOTKEYS		0xFFFFFFFF

/* Function prototypes */
/*
 * Enable/Disable UNICODE translation of keyboard input.
 * This translation has some overhead, so translation defaults off.
 * If 'enable' is 1, translation is enabled.
 * If 'enable' is 0, translation is disabled.
 * If 'enable' is -1, the translation state is not changed.
 * It returns the previous state of keyboard translation.
 */
extern DECLSPEC int SDLCALL SDL_EnableUNICODE(int enable);

/*
 * Enable/Disable keyboard repeat.  Keyboard repeat defaults to off.
 * 'delay' is the initial delay in ms between the time when a key is
 * pressed, and keyboard repeat begins.
 * 'interval' is the time in ms between keyboard repeat events.
 */
#define SDL_DEFAULT_REPEAT_DELAY	500
#define SDL_DEFAULT_REPEAT_INTERVAL	30
/*
 * If 'delay' is set to 0, keyboard repeat is disabled.
 */
extern DECLSPEC int SDLCALL SDL_EnableKeyRepeat(int delay, int interval);
extern DECLSPEC void SDLCALL SDL_GetKeyRepeat(int *delay, int *interval);

/*
 * Get a snapshot of the current state of the keyboard.
 * Returns an array of keystates, indexed by the SDLK_* syms.
 * Used:
 * 	Uint8 *keystate = SDL_GetKeyState(NULL);
 *	if ( keystate[SDLK_RETURN] ) ... <RETURN> is pressed.
 */
extern DECLSPEC Uint8 * SDLCALL SDL_GetKeyState(int *numkeys);

/*
 * Get the current key modifier state
 */
extern DECLSPEC SDLMod SDLCALL SDL_GetModState(void);

/*
 * Set the current key modifier state
 * This does not change the keyboard state, only the key modifier flags.
 */
extern DECLSPEC void SDLCALL SDL_SetModState(SDLMod modstate);

/*
 * Get the name of an SDL virtual keysym
 */
extern DECLSPEC char * SDLCALL SDL_GetKeyName(SDLKey key);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_keyboard_h */
