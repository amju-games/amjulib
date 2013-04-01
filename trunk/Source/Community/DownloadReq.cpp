#include <AmjuFirst.h>
#include "DownloadReq.h"
#include <iostream>
#include <File.h>
#include <StringUtils.h>
#include <Directory.h>
#include <AmjuFinal.h>

#define DOWNLOAD_REQ_DEBUG

#define CHECK_HTTP_RESPONSE

namespace Amju
{
void DownloadReq::HandleResult()
{
  // Had a nightmare here, the files saved were corrupt.
  // What seemed to be happening was that the vector had gone away by the time we saved the file???
  // Copying the result seems to fix the problem.
  // This makes no sense to me :-(
  HttpResult res = GetResult(); 

  if (res.GetSuccess())
  {
#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req succeeded for " << m_filename << "\n"; // GetString returns this: \"" << res.GetString() << "\"\n";
#endif

    const unsigned char* s = res.GetData();

    unsigned int size = res.Size();
    // But check the HTTP response. File data follows \r\n\r\n
    if (size < 15)
    {
#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req: HTTP response too short for " << m_filename << "\n";
#endif
      // TODO LOG bad response
      OnDownloadFailed();
      return;
    } 

    unsigned int i = 0;

#ifdef CHECK_HTTP_RESPONSE
    // Check HTTP response code
    while (i < size && s[i] != '\r') i++;
    std::string firstline(s, s + i);
    if (!StringContains(firstline, "200"))
    {
#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "First line: \"" << firstline << "\"\n";
std::cout << " - I don't see 200, response not OK for " << m_filename << " \n";
std::cout << "  Request was: " << m_url << "\n";
#endif
      // TODO LOG bad response
      OnDownloadFailed();
      return; 
    }
#endif // CHECK_HTTP_RESPONSE


    // Find \r\n\r\n
    while (i < (size - 4)  && (s[i] != '\r' || s[i + 1] != '\n' || s[i + 2] != '\r' || s[i + 3] != '\n')) i++;
    i += 4;

#ifdef DOWNLOAD_REQ_DEBUG
std::cout << "Download req " << m_filename << ": Found start of data: " << i << "\n";
std::cout << "Download req " << m_filename << ": Size of data: " << size - i << "\n";
//std::cout << "Data looks like this:\n" << std::string((const char*)(s + i)) << "\n";
#endif

    // Save response to file
    bool ok = true;

    { // so we close file before calling OnDownloaded, which may try to read it

/*
      std::ofstream of;
      of.open(m_filename.c_str(), (std::_Ios_Openmode)(std::ios_base::binary | std::ios_base::trunc));
      if (of.good())
      {  
         of.write((const char*)(s + i), size - i);    
      } 
      else
      {
        ok = false;
      }
*/

      // Do we need to create destination directory ?
      std::string dir;
      if (m_useRoot)
      {
        dir = File::GetRoot();
      }
      std::string relpath = GetFilePath(m_filename);
      if (!relpath.empty())
      {
        dir += relpath;
        // Exists ?

        // No: create
        MkDir(dir);
      }

      File f(File::NO_VERSION, File::STD);
      if (!f.OpenWrite(m_filename, File::CURRENT_VERSION, 
        true, // binary
        m_useRoot 
      ))
      {
        ok = false;
#ifdef DOWNLOAD_REQ_DEBUG
        std::cout << "Download req " << m_filename << ": Oh no, can't open file for writing :-(\n";
#endif
      }
      if (ok && !f.WriteBinary((const char*)(s + i), size - i))
      {
        ok = false;
#ifdef DOWNLOAD_REQ_DEBUG
        std::cout << "Download req " << m_filename << ": WriteBinary failed :-(\n";
#endif
      }

    }
    
    if (ok)
    {  
      OnDownloaded();
    }
    else
    {
      OnDownloadFailed();
    }
  }
  else
  {
    std::cout << "Download req for " << m_filename << " failed! " << res.GetErrorString() << "\n";
    OnDownloadFailed();
  }
}
}

