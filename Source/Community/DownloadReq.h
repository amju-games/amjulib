#ifndef DOWNLOAD_REQ_H_INCLUDED
#define DOWNLOAD_REQ_H_INCLUDED

#include "OnlineReqManager.h"

namespace Amju
{
// Downloads a file from server, saves to local disk.
// Will create a thread, so calls function when finished.
class DownloadReq : public OnlineReq
{
public:
  DownloadReq(const std::string& filename, const std::string& url, HttpClient::HttpMethod method, const char* name, bool useRoot) : 
    OnlineReq(url, method, name),
    m_filename(filename), m_useRoot(useRoot) {}

  virtual void HandleResult(); 

  virtual void OnDownloaded() {}
  virtual void OnDownloadFailed() {}

protected:
  std::string m_filename;
  bool m_useRoot;
};
}

#endif

