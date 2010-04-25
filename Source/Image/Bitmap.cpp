/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bitmap.cpp,v $
Revision 1.5  2008/05/08 10:58:03  jay
New memory management code

Revision 1.4  2006/12/30 16:21:16  jay
Fix Save() endianness

Revision 1.3  2006/12/01 23:28:03  jay
New FileImplMem, unfortunately causing many small changes

Revision 1.2  2006/01/16 13:32:20  jay
Merge fix from pool

Revision 1.1.10.1  2005/05/08 17:29:24  jay
Add <iostream> - new gcc version

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Bitmap.h"
#include "File.h" // all file access is through this class
#include "Endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <iostream>
#include <string>
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& msg);

bool IsPowerOfTwo(int i)
{
  AMJU_CALL_STACK;

  return (i == 0 || i == 1 || i == 2 || i == 4 || i == 8 || i == 16 ||
          i == 32 || i == 64 || i == 128 || i == 256 || i == 512 ||
          i == 1024 || i == 2048 || i == 4096);
}

/*
 * 'LoadDIBitmap()' - Load a DIB/BMP file from disk.
 *
 * Returns a pointer to the bitmap if successful, NULL otherwise...
 * The data is allocated on the heap with new. Ownership passes to the caller.
 */

unsigned char* LoadDIBitmap(const char *filename, unsigned int* pWidth, unsigned int* pHeight)
{
  File f(false); // false => no version info
  if (!f.OpenRead(filename, true)) // true => binary
  {
    string s = "Couldn't open .BMP file: ";
    s += filename;
    ReportError(s);
    return 0;
  }

  BitmapFileHeader  header;     
  BitmapInfo info; // info including width and height

 /*
  * Read the file header and any following bitmap information...
  */
  if (f.GetBinary(sizeof(BitmapFileHeader), (unsigned char *)&header) != sizeof(BitmapFileHeader))
  {
   /*
    * Couldn't read the file header - return NULL...
    */
    string s = "Couldn't load .BMP file header: ";
    s += filename;
    ReportError(s);
    return 0;
  }

  // look for "BM" signature
  uint8 charB = header.bytes[0];
  uint8 charM = header.bytes[1];

  if (charM != 'M' || charB != 'B') 
  {
   /*
    * Not a bitmap file - return NULL...
    */
    string s = "Apparently not a .BMP file: ";
    s += filename;
    ReportError(s);
    return 0;
  }

  // Treat bytes 10-13 of header as a uint32.
  // TODO Getting gcc warning here
  uint32 offbits = (uint32)(*(uint32*)(&header.bytes[10]));
  unsigned int infosize = Endian(offbits) - sizeof(BitmapFileHeader);
  /*
  if (infosize != sizeof(BitmapInfo))
  {
    string s = "Bad .BMP info size: ";
    s += filename;
    ReportError(s);
    return (0);
  }
  */
  if (f.GetBinary(infosize, (unsigned char *)(&info)) != infosize)
  {
   /*
    * Couldn't read the bitmap header - return NULL...
    */
    string s = "Couldn't read .BMP file header: ";
    s += filename;
    ReportError(s);

    return (0);
  }

  // j.c. Check depth is 24 bits
  uint16 bitcount = info.bmiHeader.biBitCount;
  uint16 bitsPerPixel = Endian(bitcount);

  if (bitsPerPixel != 24)
  {
    std::cout << "Bad bits per pixel!\n";

    string s = ".BMP file is not 24-bit: ";
    s += string(filename);
    ReportError(s);

    return (0);
  }

  // Set width and height.
  *pWidth = Endian(uint32(info.bmiHeader.biWidth));
  *pHeight = Endian(uint32(info.bmiHeader.biHeight));

 /*
  * Now that we have all the header info read in, allocate memory for the
  * bitmap and read *it* in...
  */
  unsigned int bitsize = 0;

  int height = Endian(uint32(info.bmiHeader.biHeight)); 
  int width = Endian(uint32(info.bmiHeader.biWidth));

  if (!IsPowerOfTwo(width) || !IsPowerOfTwo(height))
  {
    std::string err = filename;
    err += ": Bitmap is not power of two size.";
    ReportError(err);
    return 0;
  }  

  int depth = Endian(info.bmiHeader.biBitCount);
  int width32 = (width*depth/8);
  
  if (true)//(bitsize = Endian(info.bmiHeader.biSizeImage)) == 0)
  {
    int rest=(width*depth/8)%4;
    if(rest != 0)
        width32 =  (width*depth/8 + 4-rest);

    //int bitsize1 = (width * depth + 7) / 8 * height;
    int bitsize2 = width32 * height;
//    Assert(bitsize1 >= bitsize2);
    bitsize = bitsize2;
  }
  
  Assert(bitsize != 0);

  unsigned char* bits = 0;
  if (!(bits = new unsigned char[bitsize]))
  {
   /*
    * Couldn't allocate memory - return NULL!
    */
    string s = "Couldn't allocate space for .BMP file (bad size ?) : ";
    s += filename;
    ReportError(s);

    return (0);
  }
  memset(bits, 0, bitsize);

#ifndef old
  if (f.GetBinary(bitsize, (unsigned char *)bits) != bitsize)
  {
   /*
    * Couldn't read bitmap - free memory and return NULL!
    */
    string s = "Couldn't read .BMP data: ";
    s += filename;
    ReportError(s);

    delete [] bits;
    return (0);
  }
#else
  // This doesn't seem to work. 
  // TODO get this to work for any size bitmap
  for (int lines = 0; lines < height; lines++)
  {
    int w = width*depth/8;
    if (f.GetBinary(w, (unsigned char *)(bits + lines*width32)) != w)
    {
     /*
      * Couldn't read bitmap - free memory and return NULL!
      */
      string s = "Couldn't read .BMP data: ";
      s += filename;
      ReportError(s);

      delete [] bits;
      return (0);
    }
  }

#endif

	
  // Swap RGB->BGR
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char* p = bits + j * depth / 8 + i * width32;
			unsigned char c = p[0];
			p[0] = p[2];
			p[2] = c;
		}
	}
	
	
  return (bits);
}

#ifdef SCENE_EDITOR
bool SaveDIBitmap(
  const char* filename,
  const char* data,
  unsigned int w,
  unsigned int h)
{
  AMJU_CALL_STACK;

  File f(false, File::STD); // false => no version info
    // No glue, TODO MEM ?
  if (!f.OpenWrite(filename, 0, true))
    // true => binary
  {
    string s = "Couldn't open .BMP file for writing: ";
    s += filename;
    ReportError(s);
    return false;
  }

  BitmapFileHeader  header;     
  header.bytes[0] = 'B';
  header.bytes[1] = 'M';

  // File size
  *(uint32*)(&header.bytes[2]) = 
    Endian((uint32)(sizeof(BitmapFileHeader) + sizeof(BitmapInfo) + w * h * 3));

  // Reserved
  header.bytes[6] = 0;
  header.bytes[7] = 0;
  header.bytes[8] = 0;
  header.bytes[9] = 0;
  
  // Offset to data
  *(uint32*)(&header.bytes[10]) = 
    Endian((uint32)(sizeof(BitmapFileHeader) + sizeof(BitmapInfo) - 4));
  // NB maybe rgb quad is not required so 4 bytes too long.

  BitmapInfo info;
  info.bmiHeader.biSize = Endian((uint32)sizeof(BitmapInfoHeader));
  info.bmiHeader.biWidth = Endian((int32)w);
  info.bmiHeader.biHeight = Endian((int32)h);
  info.bmiHeader.biPlanes = 1;
  info.bmiHeader.biBitCount = Endian((uint16)24);
  info.bmiHeader.biCompression = 0;
  // Assume 3 bytes per pixel
  info.bmiHeader.biSizeImage = Endian((uint32)w * h * 3);
  info.bmiHeader.biXPelsPerMeter = 0;
  info.bmiHeader.biYPelsPerMeter = 0;
  info.bmiHeader.biClrUsed = 0;
  info.bmiHeader.biClrImportant = 0;


  f.WriteBinary(reinterpret_cast<const char*>(&header), 
    sizeof(header));
  // Don't write the RGB quad
  f.WriteBinary(reinterpret_cast<const char*>(&info.bmiHeader), 
    sizeof(info.bmiHeader));
  // Assume 3 bytes per pixel
  f.WriteBinary(data, w * h * 3);

  return true;
}
#endif

}


