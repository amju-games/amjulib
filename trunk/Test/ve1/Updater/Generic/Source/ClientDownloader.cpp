#include "ClientDownloader.h"
#include "Updater.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

namespace Amju
{
ClientDownloader::ClientDownloader(Updater* u, const std::string& filename, const std::string& url) : 
  DownloadReq(filename, url, HttpClient::GET, "client download", false), m_updater(u)
{ 
}

void ClientDownloader::OnDownloaded()
{
  m_updater->Report("Download succeeded!\n");

#ifndef GEKKO
  // Set permission: rwxr-xr-x
  chmod(m_filename.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif

  m_updater->OnDownloadSuccess();
}

void ClientDownloader::OnDownloadFailed()
{
  m_updater->Report("Download failed.\n");
  m_updater->Report((GetResult().GetErrorString() + "\n").c_str());

  m_updater->OnDownloadFail();
}

}

