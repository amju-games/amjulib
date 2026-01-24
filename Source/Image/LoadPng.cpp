#include <AmjuFirst.h>
#include "LoadPng.h"
#include "lodepng.h" // 3rd party
#include "png.h" // 3rd party
#include <File.h>
#include <FileImplGlue.h>
#include <GlueFile.h>
#include <ReportError.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

// Most of this code taken from LIBPNG example.c

namespace Amju
{
// C++ png loader, prefer this to the C interface.
bool LoadPng(
  const std::string& filename, 
  std::vector<unsigned char>& data,
  int& width,
  int& height)
{
  unsigned int w = 0;
  unsigned int h = 0;
  unsigned int nError = 0;

  auto gf = FileImplGlue::GetGlueFile();
  if (gf)
  {
    // Find the start and length of the png file in the glue file
    uint32 pos = 0;
    if (!gf->GetSeekBase(filename, &pos))
    {   
      std::string s = "Png: not in Glue File: " + filename;
      ReportError(s);
      return false;
    }   
    uint32 filesize = gf->GetSize(filename);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData binarydata = gf->GetBinary(pos, filesize);

    nError = lodepng::decode(
      data,
      w,
      h,
      binarydata.GetBuffer(),
      filesize); 
  }
  else
  {
    // Non-gluefile
    nError = lodepng::decode(data, w, h, filename);
  }

  if (nError != 0)
  {
    ReportError(std::string("Failed to load png, ") + lodepng_error_text(nError));
    return false;
  }

  Assert(data.size() == w * h * 4);

  width = w;
  height = h;
  return true;
}

// Version using libpng follows
// ----------------------------
static void ReadFunc(png_structp png, png_bytep data, png_size_t size)
{
  File* f = (File*)png_get_io_ptr(png); 
  unsigned int bytesRead = f->GetBinary(size, data);
  if (bytesRead != size)
  { 
    std::string err = "Read failed! bytesRead: " + ToString(bytesRead) + " size: " + ToString((unsigned int)size);
    f->ReportError(err);
  }
}

unsigned char* LoadPng(
  const char *filename, 
  unsigned int* pWidth, 
  unsigned int* pHeight,
  unsigned int* bytesPerPixel)
{
  File f(false); // false => no version info
  if (!f.OpenRead(filename, true)) // true => binary
  {
    std::string s = "Couldn't open .PNG file: ";
    s += filename;
    ReportError(s);
    return 0;
  }

  png_structp png_ptr;
  png_infop info_ptr;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  if (!png_ptr)
  {
    return 0; 
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
    return 0; 
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    return 0; 
  }

  png_set_read_fn(png_ptr, (void *)&f, ReadFunc);

  png_read_png(png_ptr, info_ptr, 0, png_voidp_NULL);

  /* At this point you have read the entire image */

  unsigned int width = png_ptr->width;
  *pWidth = width;

  unsigned int height = png_ptr->height;
  *pHeight = height;

  unsigned int bpp = png_ptr->pixel_depth >> 3;
  *bytesPerPixel = bpp;
  // or png_ptr->channels ?

  // copy pixel data into regular image format
  size_t size = width * height * bpp;
  unsigned char* data = new unsigned char[size];
  unsigned char* p = data;
  for (unsigned int i = 0; i < height; i++)
  {
    unsigned char* row = info_ptr->row_pointers[i];
    size_t rowsize = width * bpp;
    memcpy(p, row, rowsize);
    p += rowsize;
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

  return data; 
}
}
