#ifndef AMJU_LOAD_PNG_H
#define AMJU_LOAD_PNG_H

#include <string>
#include <vector>

namespace Amju
{
// C++ png loader; I would say always  prefer this to the C interface, 
//  but it always gives you RGBA, 32-bit, which is no good if we don't want
//  alpha.
bool LoadPng(
  const std::string& filename, 
  std::vector<unsigned char>& data,
  int& width,
  int& height);

// C-style interface, loads 24- and 32-bit images. 
unsigned char* LoadPng(
  const char *filename, 
  unsigned int* pWidth, 
  unsigned int* pHeight,
  unsigned int* bytesPerPixel);
}

#endif
