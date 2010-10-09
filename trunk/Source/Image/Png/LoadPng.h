#ifndef AMJU_LOAD_PNG_H
#define AMJU_LOAD_PNG_H

namespace Amju
{
unsigned char* LoadPng(
  const char *filename, 
  unsigned int* pWidth, 
  unsigned int* pHeight,
  unsigned int* bytesPerPixel);
}

#endif
