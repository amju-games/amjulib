/* BASSMIDI 2.3 C/C++ header file, copyright (c) 2006 Ian Luck.
   Requires BASS 2.3 - available from www.un4seen.com

   See the BASSMIDI.CHM file for more complete documentation */

#ifndef BASSMIDI_H
#define BASSMIDI_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSMIDIDEF
#define BASSMIDIDEF(f) WINAPI f
#endif

typedef DWORD HSOUNDFONT;	// soundfont handle

// Additional config options
#define BASS_CONFIG_MIDI_COMPACT	0x10400
#define BASS_CONFIG_MIDI_VOICES		0x10401
#define BASS_CONFIG_MIDI_AUTOFONT	0x10402

// Additional sync types
#define BASS_SYNC_MIDI_MARKER	0x10000
#define BASS_SYNC_MIDI_CUE		0x10001
#define BASS_SYNC_MIDI_LYRIC	0x10002

// additional BASS_MIDI_StreamCreateFile/etc flags
#define BASS_MIDI_DECAYEND		0x1000

typedef struct {
	HSOUNDFONT font;	// soundfont
	int preset;			// preset number (-1=all)
	int bank;
} BASS_MIDI_FONT;

typedef struct {
	const char *name;
	const char *copyright;
	const char *comment;
	DWORD presets;		// number of presets/instruments
	DWORD samsize;		// total size (in bytes) of the sample data
	DWORD samload;		// amount of sample data currently loaded
	DWORD samtype;		// sample format (CTYPE) if packed
} BASS_MIDI_FONTINFO;

typedef struct {
	DWORD pos;			// marker position
	const char *text;	// marker text
} BASS_MIDI_MARK;

// Marker types
#define BASS_MIDI_MARK_MARKER	0	// marker events
#define BASS_MIDI_MARK_CUE		1	// cue events
#define BASS_MIDI_MARK_LYRIC	2	// lyric events

// MIDI events
#define MIDI_EVENT_NOTE			1
#define MIDI_EVENT_PROGRAM		2
#define MIDI_EVENT_CHANPRES		3
#define MIDI_EVENT_PITCH		4
#define MIDI_EVENT_PITCHRANGE	5
#define MIDI_EVENT_DRUMS		6
#define MIDI_EVENT_BANK			10
#define MIDI_EVENT_MODULATION	11
#define MIDI_EVENT_VOLUME		12
#define MIDI_EVENT_PAN			13
#define MIDI_EVENT_EXPRESSION	14
#define MIDI_EVENT_SUSTAIN		15
#define MIDI_EVENT_SOUNDOFF		16
#define MIDI_EVENT_RESET		17
#define MIDI_EVENT_NOTESOFF		18

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_MIDI	0x10d00

// Additional tag type
#define BASS_TAG_MIDI_TRACK	0x11000	// + track #, track text : array of null-terminated ANSI strings


HSTREAM BASSMIDIDEF(BASS_MIDI_StreamCreate)(DWORD channels, DWORD flags, DWORD freq);
HSTREAM BASSMIDIDEF(BASS_MIDI_StreamCreateFile)(BOOL mem, const void *file, DWORD offset, DWORD length, DWORD flags, DWORD freq);
HSTREAM BASSMIDIDEF(BASS_MIDI_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, DWORD user, DWORD freq);
HSTREAM BASSMIDIDEF(BASS_MIDI_StreamCreateFileUser)(BOOL buffered, DWORD flags, STREAMFILEPROC *proc, DWORD user, DWORD freq);
DWORD BASSMIDIDEF(BASS_MIDI_StreamGetMarks)(HSTREAM handle, DWORD type, BASS_MIDI_MARK *marks, DWORD count);
BOOL BASSMIDIDEF(BASS_MIDI_StreamSetFonts)(HSTREAM handle, const BASS_MIDI_FONT *fonts, DWORD count);
DWORD BASSMIDIDEF(BASS_MIDI_StreamGetFonts)(HSTREAM handle, BASS_MIDI_FONT *fonts, DWORD count);
BOOL BASSMIDIDEF(BASS_MIDI_StreamLoadSamples)(HSTREAM handle);
BOOL BASSMIDIDEF(BASS_MIDI_StreamEvent)(HSTREAM handle, DWORD chan, DWORD event, DWORD param);

HSOUNDFONT BASSMIDIDEF(BASS_MIDI_FontInit)(const void *file, DWORD flags);
BOOL BASSMIDIDEF(BASS_MIDI_FontFree)(HSOUNDFONT handle);
BOOL BASSMIDIDEF(BASS_MIDI_FontGetInfo)(HSOUNDFONT handle, BASS_MIDI_FONTINFO *info);
BOOL BASSMIDIDEF(BASS_MIDI_FontLoad)(HSOUNDFONT handle, int preset, int bank);
BOOL BASSMIDIDEF(BASS_MIDI_FontCompact)(HSOUNDFONT handle);
BOOL BASSMIDIDEF(BASS_MIDI_FontPack)(HSOUNDFONT handle, const void *outfile, const void *encoder, DWORD flags);
BOOL BASSMIDIDEF(BASS_MIDI_FontUnpack)(HSOUNDFONT handle, const void *outfile, DWORD flags);

#ifdef __cplusplus
}
#endif

#endif
