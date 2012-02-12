#ifndef UPDATER_H_INCLUDED
#define UPDATER_H_INCLUDED

#include <Singleton.h>
#include "VersionChecker.h"
#include <ConfigFile.h>
#include <Thread.h>

namespace Amju
{
typedef void (*ReportFunc)(const char*);

class Updater : public Thread
{
public:
  Updater(ReportFunc);

  virtual void Work();

  void Download();
  
  void OnServerResponse(const std::string& latestVersion);

  void OnDownloadSuccess();

  void Wait();  
  void Unwait();

  void Report(const char*);

private:
  std::string GetServer();
  std::string ExeName();

private:
  std::string m_currentVersion;
  std::string m_latestVersion;
  bool m_downloadNewClient;
  ConfigFile m_cf;
  // Condition variable -- wait until signalled
  // TODO 
  bool m_waiting;
  ReportFunc m_report;
};

typedef Singleton<Updater> TheUpdater;
}

#endif

