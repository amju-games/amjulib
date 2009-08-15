/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

// Test harness: test saving a .png file

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "SavePng.h"
#include "File.h"
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

  File f(false /* no Amju version info */, File::STD);
  if (!f.OpenWrite("test.png", 0, true /* bin */, false))
  {
    std::cout << "Failed to open png file for writing.\n";
    return 1;
  }

  int w = 32;
  int h = 32;
  unsigned char* bgr = new unsigned char[w * h * 3];
  unsigned char* p = bgr;
  unsigned int c = 0;
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      //for (int k = 0; k < 3; k++)
      //{
      //}
      
      *p++ = (i < 16) ? 255 : 0;
      *p++ = 0;
      *p++ = (j < 16) ? 0 : 255;
    }
  }

  if (SavePng(w, h, bgr, &f))
  {
    std::cout << "Saved png file ok\n"; 
  }
  else
  {
    std::cout << "Failed to save png file. See any errors above ?\n";
  }

  delete [] bgr;

  return 0;
}


