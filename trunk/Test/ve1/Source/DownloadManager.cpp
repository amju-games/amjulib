#include <iostream>
#include "DownloadManager.h"
#include <DownloadReq.h>
#include <Directory.h>
#include <File.h>
#include "Ve1OnlineReqManager.h"

//#define DOWNLOAD_DEBUG

namespace Amju
{
class FileDownloadReq : public DownloadReq
{
public:
  FileDownloadReq(DownloadManager* dl, const std::string& filename, const std::string& url) : 
    DownloadReq(filename, url, HttpClient::GET, "file download req", true /* use root */), 
    m_dl(dl) {}

  virtual void OnDownloaded()
  {
    m_dl->SetState(m_filename, DownloadManager::AMJU_DL_LOCAL);
  }

private:
  DownloadManager* m_dl;
};


bool DownloadManager::GetFile(const std::string& filename)
{
#ifdef DOWNLOAD_DEBUG
std::cout << "GetFile: " << filename << "\n";
#endif

  State s = m_map[filename];
  switch (s)
  {
  case AMJU_DL_FAILED:
#ifdef DOWNLOAD_DEBUG
std::cout << " - DL Failed, so no!??\n";
#endif
    return false;

  case AMJU_DL_UNKNOWN:
    if (FileExists(File::GetRoot() + filename))
    {
#ifdef DOWNLOAD_DEBUG
std::cout << " - File exists.\n";
#endif
      m_map[filename] = AMJU_DL_LOCAL;
      return true;
    }
    else
    {
#ifdef DOWNLOAD_DEBUG
std::cout << " - File not local, downloading...\n";
#endif
      OnlineReq* filedownloadreq = new FileDownloadReq(
        this, filename, TheVe1ReqManager::Instance()->MakeDownloadUrl(filename));

      m_map[filename] = AMJU_DL_DOWNLOADING;
      if (TheVe1ReqManager::Instance()->AddReq(
          filedownloadreq, MAX_CONCURRENT_DOWNLOADS))
      {
        return true;
      }
      else
      {
std::cout << " - File not local, download failed (too many requests ?)\n";
        m_map[filename] = AMJU_DL_UNKNOWN;
        //Assert(0);
        return false;
      }
    }

  case AMJU_DL_LOCAL:
#ifdef DOWNLOAD_DEBUG
std::cout << " - Should be local.\n";
#endif
    return true;

  case AMJU_DL_DOWNLOADING:
#ifdef DOWNLOAD_DEBUG
std::cout << " - is still downloading.\n";
#endif
    return true;

  }
  Assert(0);
  return true;
}

bool DownloadManager::IsLocal(const std::string& filename)
{
  return m_map[filename] == AMJU_DL_LOCAL;
}

void DownloadManager::SetState(const std::string& filename, State s)
{
  m_map[filename] = s;
}

}
