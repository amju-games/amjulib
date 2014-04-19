#include <iostream>
#include <AmjuGL-Null.h>
#include <Bitmap.h>
#include <StringUtils.h>
#include <SavePng.h>
#include <TextureUtils.h>

using namespace Amju;

int main(int argc, char** argv)
{
  if (argc != 2 && argc != 3)
  {
    std::cout << "bmp2png converts .bmp files to .png files. Only works for 24-bit bmps.\nUsage: bmp2png <bmpfile> [<optional alpha bmpfile>]\n";
    return 1;
  }

  AmjuGL::SetImpl(new AmjuGLNull);

  std::string inFilename = argv[1]; 

  unsigned int w = 0;
  unsigned int h = 0;
  int bytesPerPixel = 3;

  unsigned char* img = LoadDIBitmap(argv[1], &w, &h);

  if (img)
  {
    std::cout << "Loaded " << inFilename << " ok. Width: " << w << " Height: " << h << "\n";
  }
  else
  {
    std::cout << "Failed to load " << inFilename << "\n";
    return 1;
  }

  if (argc == 3)
  {
    // second param is another bmp file which is used as alpha channel.
    unsigned int aw = 0;
    unsigned int ah = 0;
    unsigned char* alpha = LoadDIBitmap(argv[2], &aw, &ah);

    if (alpha)
    {
      std::cout << "Loaded " << argv[2] << " ok. Width: " << w << " Height: " << h << "\n";
    }
    else
    {
      std::cout << "Failed to load " << argv[2] << "\n";
      return 1;
    }

    if (aw != w || ah != h)
    {
      std::cout << "Both bmps must be the same size!\n";
      return 1; 
    }

    // Add alpha channel to img
    // TODO These functions are poorly named and have no error checking
    unsigned char* newimg = AddAlpha(img, w, h);
    delete [] img;
    img = newimg;

//    CopyAlpha(alpha, img, w, h);
//    std::cout << "Combined bitmaps. \n";  

std::cout << "Img is now 4 bytes per pixel, but didn't add alpha data.\n";
    bytesPerPixel = 4;
  }

  std::cout << "Saving as .png, " << bytesPerPixel << " bytes per pixel\n";

  std::string outFilename = GetFileNoExt(inFilename);
  outFilename += ".png";

  bool b = SavePng(w, h, img, outFilename.c_str(), bytesPerPixel);

  if (b)
  {
    std::cout << "Saved " << outFilename << " ok!\n";
  } 
  else
  {
    std::cout << "Failed to save " << outFilename << " :-(\n";
    // TODO Get error msg
    return 1;
  }

  return 0;
}
