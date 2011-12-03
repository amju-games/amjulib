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
  DownloadReq(const std::string& filename, const std::string& url, HttpClient::HttpMethod method, const char* name) : 
    OnlineReq(url, method, name),
    m_filename(filename) {}

  virtual void HandleResult(); 

  virtual void OnDownloaded() {}

protected:
  std::string m_filename;
};
}

#endif

