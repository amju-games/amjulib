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
static const char* VERSION_SCRIPT = "/ve1/cgi-bin/version.pl"; 
static const char* SERVER_EXE_PATH = "/ve1/assets/";
static const char* CONFIG_FILE_NAME = "client_version.txt";
static const char* APPLICATION = "ve1";
static const char* VERSION_KEY = "version";
static const char* SERVER_KEY = "server";
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
  if (m_cf.Load(CONFIG_FILE_NAME))
  {
    Report("Loaded config file.\n");
  }
  else
  {
    Report("No config file.\n");
  }
}

void Updater::Report(const char* c)
{
	m_report(c);
}

std::string Updater::ExeName()
{
  return CLIENT_PREFIX + m_currentVersion + CLIENT_SUFFIX;
}

void Updater::Download()
{
  std::string clientExeName = ExeName();

  std::string url = GetServer() + SERVER_EXE_PATH + clientExeName;

  Report(std::string("Downloading \"" + clientExeName + "\"...\n").c_str()); 

std::cout << "URL: " << url << "\n";

  ClientDownloader* cd = new ClientDownloader(this, clientExeName, url);
  m_waiting = true;
  cd->Work();
  Wait();
    
  // TODO Check download result. Display error msg to user if download failed.
}


void Updater::OnDownloadSuccess()
{
  // TODO Set success flag

  m_currentVersion = m_latestVersion;
  // Update version in config file once downloaded
  m_cf.Set(VERSION_KEY, m_currentVersion);
  m_cf.Save(CONFIG_FILE_NAME);
}

void Updater::OnServerResponse(const std::string& latest)
{
  Unwait();

  m_latestVersion = latest;

  Report(std::string("Local version: " + m_currentVersion + ", latest version on server: " + m_latestVersion + "\n").c_str());

  Strings strs[2] = { Split(m_currentVersion, '.'), Split(latest, '.') };
  if (strs[0].size() == strs[1].size())
  {
    int size = strs[0].size();
    for (int i = 0; i < size; i++)
    {
      if (strs[0][i] != strs[1][i])
      {
        m_downloadNewClient = true; 
        break;
      }
    }
   }
  else
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

void Updater::Work()
{
  File::SetRoot(GetSaveDir(APPLICATION), "/"); // all assets, downloaded files etc live in save dir

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

std::cout << "Getting latest version info from server...\n";

  // Create downloader to get latest version on server
  std::string url = GetServer() + VERSION_SCRIPT;
  VersionChecker* v = new VersionChecker(this, url);
  m_waiting = true;
  v->Work();
  Wait();

//std::cout << "Got latest version info from client.\n";

  if (m_downloadNewClient)
  {
    Download();
  }

std::cout << "Now it's time to start the client and exit this process.\n";
  std::string exe = File::GetRoot() + ExeName();
std::cout << "Exe: " << exe << "\n";

  if (FileExists(exe))
  {
    if (!LaunchProcess(exe.c_str()))
    {
std::cout << "File exists but can't run it. Try to download it again, so reset current version..?\n";
    }
  }
  else
  {
std::cout << "The current version of the exe doesn't exist!? Can't run it :-(\n";
  }
}

void Updater::Wait()
{
std::cout << "Waiting... ";
  // TODO Wait until signalled -- but watch out for race conditions --- could the signal alreay have happened ?!?
  while (m_waiting)
  {
//    std::cout << ".";
    SleepMs(1000);
  }
  std::cout << "\n";
}

void Updater::Unwait()
{
  m_waiting = false;
}
}

