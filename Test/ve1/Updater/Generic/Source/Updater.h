#ifndef UPDATER_H_INCLUDED
#define UPDATER_H_INCLUDED

#include <Singleton.h>
#include "VersionChecker.h"
#include <ConfigFile.h>
#include <ThreadGate.h>

namespace Amju
{
class Updater
{
public:
  void Start();

  void Download();
  
  void OnServerResponse(const std::string& latestVersion);

  void OnDownloadSuccess();

  void Wait();  
  void Unwait();

private:
  Updater();
  friend class Singleton<Updater>;

  std::string GetServer();
  std::string ExeName();

private:
  std::string m_version;
  bool m_downloadNewClient;
  ConfigFile m_cf;
  // Condition variable -- wait until signalled
  // TODO 
  bool m_waiting;
};

typedef Singleton<Updater> TheUpdater;
}

#endif

