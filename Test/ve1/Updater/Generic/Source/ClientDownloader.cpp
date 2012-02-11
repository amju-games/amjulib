#include "ClientDownloader.h"
#include "Updater.h"
#include <iostream>

namespace Amju
{
ClientDownloader::ClientDownloader(const std::string& filename, const std::string& url) : 
  DownloadReq(filename, url, HttpClient::GET, "client download")
{ 
}

void ClientDownloader::OnDownloaded()
{
std::cout << "Download succeeded!\n";

  TheUpdater::Instance()->OnDownloadSuccess();
}

void ClientDownloader::OnDownloadFailed()
{
std::cout << "Download failed!\n";

  TheUpdater::Instance()->Unwait();
}

}

