#ifndef CLIENT_DOWNLOADER_H_INCLUDED
#define CLIENT_DOWNLOADER_H_INCLUDED

#include <DownloadReq.h>

namespace Amju
{
class Updater;

class ClientDownloader : public DownloadReq
{
public:
  ClientDownloader(Updater* u, const std::string& filename, const std::string& url);
  virtual void OnDownloaded();
  virtual void OnDownloadFailed();

private:
  Updater* m_updater;
};
}

#endif

