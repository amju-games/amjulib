#include "Download2.h"
#include "GSFileUpdateCheck.h"

namespace Amju
{
class MyDownloadReq : public DownloadReq
{
public:
  MyDownloadReq(const std::string& filename, const std::string& url) : 
    DownloadReq(filename, url, HttpClient::GET, "file download req", true /* use root */)
  {
  }

  virtual void OnDownloaded() 
  {
    TheGSFileUpdateCheck::Instance()->OnDownloadedFile(m_filename);
  }

  virtual void OnDownloadFailed() 
  {
    // Bummer - best finish downloading
    Assert(0);
    TheGSFileUpdateCheck::Instance()->OnSkip();
  }
};

bool DownloadFile(const std::string& filename)
{
  OnlineReq* filedownloadreq = new MyDownloadReq(
    filename, TheVe1ReqManager::Instance()->MakeDownloadUrl(filename));

  static const int MAX_CONCURRENT_DOWNLOADS = 1000; // TODO
  if (TheVe1ReqManager::Instance()->SerialReqManager::AddReq(filedownloadreq, MAX_CONCURRENT_DOWNLOADS))
  {
    return true;
  }
  else
  {
    Assert(0);
    return false;
  }
}

}
