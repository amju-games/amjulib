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
  ~Updater();

  virtual void Work();

  void Download();
  
  void OnServerResponse(const std::string& latestVersion);
  void OnBadServerResponse();

  void OnDownloadSuccess();
  void OnDownloadFail();

  void Report(const char*);

private:
  std::string GetServer();
  std::string GetEnv();
  std::string ExeName(const std::string& version);
  void CopyBaseResources();

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

