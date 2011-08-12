#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED

// Mostly useful for Wii debugging.
// If used on MacOSX, you need to link to libcurses: add  -lcurses to link line

namespace Amju
{
void Pause(const char* funcName, const char* fileName, unsigned int line);

#define PAUSE Pause(__FUNCTION__, __FILE__, __LINE__);
}

#endif

