#include <AmjuFirst.h>
#include "TextureUtils.h"
#include "AmjuAssert.h"
#ifdef GEKKO
#include <malloc.h> // memalign
#include <string.h> // memcpy ??!
#endif
#include <iostream>
#include <AmjuFinal.h>

namespace Amju
{
void SwapRow(uint8* data, int w, int h1, int h2, uint8* row, int bytesPerPixel)
{
  // memcpy(dest, src, size)
  // From h1 to row
  memcpy(row, data + h1 * w * bytesPerPixel, w * bytesPerPixel);

  // From h2 to h1
  memcpy(data + h1 * w * bytesPerPixel, data + h2 * w * bytesPerPixel, w * bytesPerPixel);

  // From row to h2
  memcpy(data + h2 * w * bytesPerPixel, row, w * bytesPerPixel);
}

void FlipBmp(uint8* data, int w, int h, int bytesPerPixel)
{
  uint8* row = new uint8[w * bytesPerPixel];
  int h2 = h >> 1;
  for (int i = 0; i < h2; i++)
  {
    // Swap row i and h - i
    SwapRow(data, w, i, h - i - 1, row, bytesPerPixel);
  }
  delete [] row;
}

uint8* AddAlpha(uint8* source, int w, int h)
{
  int size = w * h;
  Assert(size > 0);
  uint8* dest = new uint8[4 * size];
  Assert(dest);

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      int n = i * w + j;

      dest[n * 4 + 0] = source[n * 3 + 0]; 
      dest[n * 4 + 1] = source[n * 3 + 1]; 
      dest[n * 4 + 2] = source[n * 3 + 2]; 
      dest[n * 4 + 3] = 0xff; 
    }
  }

/*
  int BytePerPixel = 3;
  for(int i = 0; i < size; i++)
  {
    dest[4*i+0] = source[BytePerPixel*i+0];
    dest[4*i+1] = source[BytePerPixel*i+1];
    dest[4*i+2] = source[BytePerPixel*i+2];
    dest[4*i+3] = 0xff;
  }
*/

  return dest;
}

void CopyAlpha(const uint8* source, uint8* dest, int w, int h)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      // Only R channel used for alpha
      dest[(i * w + j) * 4 + 3] = source[(i * w + j) * 3]; 
    }
  }
}
}
