/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bitmap.h,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

// Load a Windows .BMP bitmap. 
// I don't know where the original code came from.
// Adapted from the original code by j.c. for portability.

/*
 * "$Id: Bitmap.h,v 1.1 2004/09/08 15:43:17 jay Exp $"
 *
 *   Device Independent Bitmap definitions for OpenGL under MS Windows.
 *
 * Revision History:
 *
 *   $Log: Bitmap.h,v $
 *   Revision 1.1  2004/09/08 15:43:17  jay
 *   Added to repository
 *
 *   Revision 1.2  1996/01/09  22:52:53  mike
 *   Added PrintDIBitmap.
 *
 *   Revision 1.1  1995/12/31  07:26:54  mike
 *   Initial revision
 */

#include "Common.h" // portable data types

namespace Amju
{
// Portable definitions of structs in <windows.h>
struct BitmapInfoHeader
{
  uint32      biSize;
  int32       biWidth;
  int32       biHeight;
  uint16      biPlanes;
  uint16      biBitCount;
  uint32      biCompression;
  uint32      biSizeImage;
  int32       biXPelsPerMeter;
  int32       biYPelsPerMeter;
  uint32      biClrUsed;
  uint32      biClrImportant;
};

struct RgbQuad 
{
  uint8    rgbBlue;
  uint8    rgbGreen;
  uint8    rgbRed;
  uint8    rgbReserved;
};

struct BitmapInfo
{
  BitmapInfoHeader    bmiHeader;
  RgbQuad             bmiColors[1];
};

// Windows uses #pragma pack(2) with this struct. To avoid alignment issues, we
// declare an array of bytes rather than individual members.
struct BitmapFileHeader
{
  /*
  uint16    bfType;
  uint32    bfSize;
  uint16    bfReserved1;
  uint16    bfReserved2;
  uint32    bfOffBits;
  */
  uint8 bytes[14];
};

// Load a bitmap.
// If sucessful, returns a pointer to the bitmap data, and sets the width and height.
// The data is an array allocated with new[]. Ownership passes to the caller.
unsigned char* LoadDIBitmap(const char *filename, unsigned int* pWidth, unsigned int* pHeight);

// j.c. 5/6/2003 Save a 24-bit Texture in .bmp format.
#if defined(SCENE_EDITOR)
bool SaveDIBitmap(
  const char* filename,
  const char* data,
  unsigned int w,
  unsigned int h);
#endif

}

#endif 
