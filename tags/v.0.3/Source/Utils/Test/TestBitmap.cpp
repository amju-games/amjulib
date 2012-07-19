/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

// Test harness:
// Loads the .bmp file given as argument.

#include <iostream>
#include <string>
#include "Bitmap.h"
#include "AmjuFinal.h"

using namespace Amju;

namespace Amju
{
void ReportError(const std::string& s)
{
  AMJU_CALL_STACK;

  std::cout << s.c_str() << "\n";
}
}

int main(int argc, char** argv)
{
  AMJU_CALL_STACK;

  if (argc != 2)
  {
    std::cout << "Usage: testbitmap <bitmap file>\n";
    exit(1);
  }

  std::string filename = argv[1];

  unsigned int w, h;
  if (LoadDIBitmap(filename.c_str(), &w, &h))
  {
    std::cout << "Loaded bitmap ok! W: " << w << " H: " << h << "\n";
  }
  else
  {
    std::cout << "FAILED to load bitmap!\n";
  }
  return 0;
}


