#ifndef CLIENT_DOWNLOADER_H_INCLUDED
#define CLIENT_DOWNLOADER_H_INCLUDED

#include <DownloadReq.h>

namespace Amju
{
class ClientDownloader : public DownloadReq
{
public:
  ClientDownloader(const std::string& filename, const std::string& url);
  virtual void OnDownloaded();
  virtual void OnDownloadFailed();
};
}

#endif

