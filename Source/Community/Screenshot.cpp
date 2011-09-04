/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#include "AmjuFirst.h"

// Save .bmp for test/comparison -- blue and red are reversed though :-(
//#define SAVE_AS_BMP

#include <iostream>

#include "Screenshot.h"
#include <SavePng.h>
#include <AmjuGL.h>
#include <Screen.h>

#ifdef SAVE_AS_BMP
#include <Bitmap.h>
#endif

#include <File.h>
#include <Directory.h>
#include <HttpClient.h>
#include <UrlUtils.h>
#include <Thread.h>
#include <RCPtr.h>
#include <AmjuAssert.h>
#include "AmjuFinal.h"

namespace Amju
{
void SaveScreenshot(const std::string& filename, int s)
{
  AMJU_CALL_STACK;

  static const int BYTES_PER_PIXEL = 3;

  int w = Screen::X();
  int h = Screen::Y();
  unsigned char* mem = new unsigned char[BYTES_PER_PIXEL * w * h];

  // NB Must do this in main thread with GL context
  ////glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, mem);
  AmjuGL::GetScreenshot(mem, w, h);

  // TODO Save in a separate thread 

  // Shrink image 
  // TODO This should go in some kind of image processing module

  // s is number of times smaller to make the image
  // I.e. new width will be width/s; new height will be height/s.
  int ssq = s * s;
  // Can't allocate on stack in MSVC, size must be known
  uint8** c = new uint8*[ssq];

  int w2 = w / s;
  int h2 = h / s;

  // TODO Add border for s==3 etc
  int smallerImageSize = BYTES_PER_PIXEL * (w2 + s - 1) * (h2 + s - 1);
  unsigned char* smallerImage = new unsigned char[smallerImageSize];
  // Nested loop. Each pixel in the smaller image is the average of a 
  //  square of s * s pixels
  for (int i = 0; i < h; i+= s)
  {
    for (int j = 0; j < w; j+= s)
    {
      // Get the pixels in the square which we will average.
      // c is an array of pointers to pixel data, not the data itself
      //uint8* c[ssq];
      for (int m = 0; m < s; m++)
      {
        for (int n = 0; n < s; n++)
        {
          c[m * s + n] = mem + BYTES_PER_PIXEL * ((i + m) * w + j + n);
        }
      }

      uint32 rgb[BYTES_PER_PIXEL]; // larger than uint8, for overflow
      // For each colour component
      for (int k = 0; k < BYTES_PER_PIXEL; k++)
      {
        rgb[k] = 0;
        for (int p = 0; p < ssq; p++)
        {
          // Average colour of s * s square: sum of colours, divided by s * s 
          rgb[k] += *(c[p] + k);
        }
        rgb[k] /= ssq;
        Assert(!(rgb[k] & 0xffffff00));

        int q = BYTES_PER_PIXEL * ((i / s) * w2 + (j / s));
        Assert(q + k < smallerImageSize);
        smallerImage[q + k] = (uint8)(rgb[k]);
      }
    }
  } 

  delete [] c;

#ifdef _DEBUG
std::cout << "Saving png image: " << filename.c_str() << " width: " << w2 << " height: " << h2 << "\n";
#endif

//  bool savedPngOk = SavePng(w, h, mem, filename.c_str());
  bool savedPngOk = SavePng(w2, h2, smallerImage, filename.c_str(), BYTES_PER_PIXEL);

  if (savedPngOk)
  {
#ifdef SCREENSHOT_DEBUG
std::cout << "SAVED SCREENSHOT AS PNG!\n";
#endif
  }
  else
  {
#ifdef SCREENSHOT_DEBUG
std::cout << "FAILED TO SAVE SCREENSHOT AS PNG!\n";
#endif
  }

#ifdef SAVE_AS_BMP
  // Just out of interest, here is how to save as BMP - which is
  //  stupid as the filesize is so huge.
  std::string bmpFilename = filename + ".bmp";
  if (SaveDIBitmap("bmtest.bmp", (char*)smallerImage, w2, h2))
  {
std::cout << "SAVED BITMAP!\n";
  }
  else
  {
std::cout << "FAILED TO SAVE BITMAP!\n";
  }
#endif

  delete [] mem;
  delete [] smallerImage;
}

class UploadThread : public Thread
{
public:
  UploadThread(const std::string& filename, const std::string& url) : m_filename(filename), m_url(url) {}
  virtual void Work();

private:
  std::string m_filename, m_url;
};

void UploadScreenshot(const std::string& filename, const std::string& url)
{
  RCPtr<Thread> thread = new UploadThread(filename, url);
  thread->Start();
}

void UploadThread::Work()
{
  if (!FileExists(m_filename))
  {
#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: Apparently there is no file '"
  << m_filename.c_str() << "'\n";
#endif
    return;
  }

  File pngFile(false, File::STD);
  if (!pngFile.OpenRead(m_filename, true, false))
  {
#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: failed to open file '"
  << m_filename.c_str() << "'\n";
#endif
  }

#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: Opened file '" << m_filename.c_str() << "'...\n";
#endif

  m_url += "&image=";

  static const unsigned int BUF_SIZE = 4096; // TODO CONFIG

  unsigned char* data = new unsigned char[BUF_SIZE];
  // Read png file until we get to the end.
  unsigned int total = 0;
  while (true)
  { 
    unsigned int bytesRead = pngFile.GetBinary(BUF_SIZE, data);
    total += bytesRead;

#ifdef _DEBUG
std::cout << "UPLOAD IMAGE:  ..read " << bytesRead << " bytes, total: "
  << total << "\n";
#endif

    m_url += ToUrlFormat(data, bytesRead);
    if (bytesRead < BUF_SIZE)
    {
      break;
    }
  }
  delete [] data;

std::cout << "Here is the complete url:\n" << m_url << "\n";

  HttpClient hc;
  HttpResult res = hc.Get(m_url, HttpClient::POST); // POST for big data, right ?
  if (res.GetSuccess())
  {
    std::cout << res.GetString().c_str() << "\n";
  }
  else
  {
    std::cout << "Failed. Error: " << res.GetErrorString().c_str() << "\n";
  }

}
}
