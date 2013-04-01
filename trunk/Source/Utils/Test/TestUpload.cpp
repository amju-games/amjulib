/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

// Test harness: Upload a .png file

#include <AmjuFirst.h>
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "File.h"
#include "HttpClient.h"
#include "StringUtils.h"
#include "UrlUtils.h"
#include <AmjuFinal.h>

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
    std::cout << "UploadTest <file.png>\n";
    exit(1);
  }

  std::string imgFilename = argv[1]; 
/*
  if (!FileExists(imgFilename))
  {
#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: Apparently there is no file '"
  << imgFilename.c_str() << "'\n";
#endif
    return 1;
  }
*/

  File pngFile(false, File::STD);
  if (!pngFile.OpenRead(imgFilename, true, false))
  {
#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: failed to open file '"
  << imgFilename.c_str() << "'\n";
#endif
    return 1;
  }

#ifdef _DEBUG
std::cout << "UPLOAD IMAGE: Opened file '" << imgFilename.c_str() << "'...\n";
#endif

  std::string url = "www.amju.com/cgi-bin/apz-online.pl";
  url += "?task=4";
  url += "&playerid=";
  url += ToString(999);
  url += "&image=";

  static const unsigned int BUF_SIZE = 200;
    //(int)Engine::Instance()->GetConfigFloat(
    //"upload-buf-size");

  unsigned char data[BUF_SIZE];
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

    url += ToUrlFormat(data, bytesRead);
    if (bytesRead < BUF_SIZE)
    {
      break;
    }
  }


#ifdef _DEBUG
std::cout << "UPLOAD IMAGE:  ..finished reading png file.\n";
#endif

  HttpClient hc;
  HttpResult res = hc.Get(url, HttpClient::POST);

std::cout << res.GetString().c_str() << "\n";;
  if (res.GetSuccess())
  {
std::cout << "OK!\n";
  }
  else
  {
std::cout << "Error: " << res.GetErrorString().c_str() << "\n";
  }
  return 0;
}


