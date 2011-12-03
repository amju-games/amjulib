#include "DownloadReq.h"
#include <iostream>
#include <File.h>
#include <StringUtils.h>

namespace Amju
{
void DownloadReq::HandleResult()
{
  if (GetResult().GetSuccess())
  {
#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req succeeded! GetString returns this: \"" << GetResult().GetString() << "\"\n";
#endif

    const unsigned char* s = GetResult().GetData();

    unsigned int size = GetResult().Size();
    // But check the HTTP response. File data follows \r\n\r\n
    if (size < 15)
    {
#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req: HTTP response too short\n";
#endif
      // TODO LOG bad response
      return;
    } 

    unsigned int i = 0;

    // TODO Check HTTP response code
/*
    // This doesn't work, TODO 
    while (i < size && s[i] != '\r') i++;
    std::string firstline(s, s + i);
std::cout << "First line: \"" << firstline << "\"\n";
    if (!StringContains(firstline, "200"))
    {
std::cout << " - I don't see 200, response not OK :-( \n";
      // TODO LOG bad response
      return; 
    }
*/

    // Find \r\n\r\n
    while (i < (size - 4)  && (s[i] != '\r' || s[i + 1] != '\n' || s[i + 2] != '\r' || s[i + 3] != '\n')) i++;
    i += 4;

#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req: Found start of data: " << i << "\n";
std::cout << "Download req: Size of data: " << size - i << "\n";
#endif

    // Save response to file
    bool ok = true;

    { // so we close file before calling OnDownloaded, which may try to read it

      File f(File::NO_VERSION, File::STD);
      if (!f.OpenWrite(m_filename, File::CURRENT_VERSION, true /* binary */))
      {
        ok = false;
#ifdef DOWNLOAD_REQ_DEBUG
        std::cout << "Download req: Oh no, can't open file for writing :-(\n";
#endif
      }
      if (ok && !f.WriteBinary((const char*)(s + i), size - i))
      {
        ok = false;
#ifdef DOWNLOAD_REQ_DEBUG
        std::cout << "Download req: WriteBinary failed :-(\n";
#endif
      }
    }
    
    if (ok)
    {  
      OnDownloaded();
    }
  }
  else
  {
    std::cout << "Download req failed! " << GetResult().GetErrorString() << "\n";
  }
}
}

