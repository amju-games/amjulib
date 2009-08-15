#ifndef TEXTURE_UTILS_H_INCLUDED
#define TEXTURE_UTILS_H_INCLUDED

#include "AmjuTypes.h"

namespace Amju
{
// Convert RGB texture data to RGBA
// Returns ptr to new array allocated with new[]
uint8* AddAlpha(uint8* source, int w, int h); 

// Copy alpha data from source to alpha channel in dest.
// Dest must be 4 in RGBA format.
void CopyAlpha(const uint8* source, uint8* dest, int w, int h);

// Flips rows, so top row becomes the bottom row, etc.
// Useful if your bmp image is upside-down!
void FlipBmp(uint8* data, int w, int h, int bytesPerPixel);
}

#endif
