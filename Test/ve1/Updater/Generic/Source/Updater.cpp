#include "Updater.h"
#include <StringUtils.h>
#include <File.h>
#include <Directory.h>
#include "VersionChecker.h"
#include <iostream>
#include "ClientDownloader.h"
#include <Launcher.h>
#include <AmjuSleep.h>

namespace Amju
{
static const char* DEFAULT_SERVER = "www.amju.com";
static const char* DEFAULT_ENV = "ve1";
static const char* VERSION_SCRIPT = "/cgi-bin/version.pl"; 
static const char* SERVER_EXE_PATH = "/bin/";
static const char* CONFIG_FILE_NAME = "client_version.txt";
static const char* VERSION_KEY = "version";
static const char* SERVER_KEY = "server";
static const char* ENV_KEY = "env";
static const char* CLIENT_PREFIX = "ve1-client-";
static const char* CLIENT_SUFFIX = 
#ifdef WIN32
".exe";
#endif
#ifdef MACOSX
"";
#endif

Updater::Updater(ReportFunc r) : m_currentVersion("0.0.0"), m_downloadNewClient(false), m_waiting(false), m_report(r)
{
  // The config file should be loaded from the directory this program lives in, so we can change the env.
  // This is called before we set the File root.
  std::string configFileName = GetProcessDir() + "/" + CONFIG_FILE_NAME;

std::cout << "Config file: " << configFileName << "\n";

  if (m_cf.Load(configFileName))
  {
    Report(std::string("Loaded config file: " + configFileName + "\n").c_str());
  }
  else
  {
    Report(std::string("No config file: " + configFileName + "\n").c_str());
  }
}

void Updater::Report(const char* c)
{
  m_report(c);
}

std::string Updater::ExeName(const std::string& version)
{
  Assert(!version.empty());
  return CLIENT_PREFIX + version + CLIENT_SUFFIX;
}

void Updater::Download()
{
  std::string clientExeName = ExeName(m_latestVersion);

  std::string url = GetServer() + "/" + GetEnv() + SERVER_EXE_PATH + clientExeName;

  Report(std::string("Downloading \"" + clientExeName + "\"...\n" +
    "URL: " + url + "\n").c_str()); 

  std::string dest = GetSaveDir(GetEnv()) + "/" + clientExeName;

  Report(std::string("Destination: " + dest + "\n").c_str());

  ClientDownloader* cd = new ClientDownloader(this, dest, url);
  m_waiting = true;
  cd->Work(); // So this is also in the current thread
  //Wait();
}

void Updater::OnDownloadSuccess()
{
  // TODO Set success flag

  Report("Successfully downloaded - saving new version info in config file.");

  m_currentVersion = m_latestVersion;
  // Update version in config file once downloaded
  m_cf.Set(VERSION_KEY, m_currentVersion);
  m_cf.Save(CONFIG_FILE_NAME);
}

void Updater::OnDownloadFail()
{
  //Report("Download failed."); // Calling code said this already
}

void Updater::OnServerResponse(const std::string& latest)
{
  m_latestVersion = latest;

  Report(std::string("Local version: " + m_currentVersion + ", latest version on server: " + 
    m_latestVersion + "\n").c_str());

  if (m_latestVersion != m_currentVersion)
  {
    m_downloadNewClient = true; 
  }

  if (m_downloadNewClient)
  {
    Report("So I will download new client...\n");
  }
  else
  {
    Report("So no need to download new client.\n");
  }
  //Unwait();
}

void Updater::OnBadServerResponse()
{
  m_downloadNewClient = false;
  Report("Bad server response, so will not try to download client.\n");
  //Unwait();
}

std::string Updater::GetServer()
{
  std::string server = DEFAULT_SERVER;
  if (m_cf.Exists(SERVER_KEY))
  {
    server = m_cf.GetValue(SERVER_KEY);
  }
  return server;
}

std::string Updater::GetEnv()
{
  std::string env = DEFAULT_ENV;
  if (m_cf.Exists(ENV_KEY))
  {
    env = m_cf.GetValue(ENV_KEY);
  }
  return env;
}

void Updater::Work()
{
  File::SetRoot(GetSaveDir(GetEnv()), "/"); // all assets, downloaded files etc live in save dir

  m_downloadNewClient = false;

  // Compare versions
  std::string version;
  if (m_cf.Exists(VERSION_KEY))
  {
    m_currentVersion = m_cf.GetValue(VERSION_KEY);
    Report(std::string("Got current client version: " + m_currentVersion + "\n").c_str());
  }
  else
  {
    m_downloadNewClient = true;
    Report("I don't know the current client version, so will download.\n");
  }

  Report("Getting latest version info from server...\n");

  // Create downloader to get latest version on server
  std::string url = GetServer() + "/" + GetEnv() + VERSION_SCRIPT;
  
  Report(std::string("URL: " + url + "\n").c_str());

  VersionChecker* v = new VersionChecker(this, url);
  m_waiting = true;
  v->Work(); // So in this thread, not a new thread
 
std::cout << "VersionChecker has finished.\n";

  // We may have got latest version number from server, or may have got no response.
  //Report("Got latest version info from client.\n");

  if (m_downloadNewClient)
  {
    Download();
  }

  Report("Now it's time to start the client and exit this process.\n");

  std::string exe = File::GetRoot() + ExeName(m_currentVersion);

  Report(std::string("Exe: " + exe + "\n").c_str());

  if (FileExists(exe))
  {
    if (!LaunchProcess(exe.c_str()))
    {
      Report("Executable file exists but can't run it.\n"); // TODO Error msg

      // Try to download it again, so reset current version..?
    }
  }
  else
  {
    Report("The current version of the exe doesn't exist!? Can't run it :-(\n");
  }
}
}

