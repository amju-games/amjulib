#include "DownloadManager.h"
#include <DownloadReq.h>
#include <Directory.h>
#include <File.h>
#include "Ve1OnlineReqManager.h"

namespace Amju
{
// TODO
static const int MAX_CONCURRENT_DOWNLOADS = 16;  // ???


class FileDownloadReq : public DownloadReq
{
public:
  FileDownloadReq(DownloadManager* dl, const std::string& filename, const std::string& url) : 
    DownloadReq(filename, url, HttpClient::GET, "file download req"), 
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
  State s = m_map[filename];
  switch (s)
  {
  case AMJU_DL_FAILED:
    return false;

  case AMJU_DL_UNKNOWN:
    if (FileExists(File::GetRoot() + filename))
    {
      m_map[filename] = AMJU_DL_LOCAL;
      return true;
    }
    else
    {
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
        m_map[filename] = AMJU_DL_FAILED;
        Assert(0);
        return false;
      }
    }

  case AMJU_DL_LOCAL:
    return true;

  case AMJU_DL_DOWNLOADING:
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
