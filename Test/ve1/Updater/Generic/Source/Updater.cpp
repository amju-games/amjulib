#include "Updater.h"
#include <StringUtils.h>
#include <File.h>
#include <Directory.h>
#include "VersionChecker.h"
#include <iostream>
#include "ClientDownloader.h"
#include <Launcher.h>
#include <unistd.h>

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

Updater::Updater() : m_version("0.0.0"), m_downloadNewClient(false), m_waiting(false)
{
  if (m_cf.Load(CONFIG_FILE_NAME))
  {
std::cout << "Loaded config file.\n";
  }
  else
  {
std::cout << "No config file.\n";
  }
}

std::string Updater::ExeName()
{
  return CLIENT_PREFIX + m_version + CLIENT_SUFFIX;
}

void Updater::Download()
{
  std::string clientExeName = ExeName();

  std::string url = GetServer() + SERVER_EXE_PATH + clientExeName;

std::cout << "Downloading \"" << clientExeName << "\"...\n"; 
std::cout << "URL: " << url << "\n";

  ClientDownloader* cd = new ClientDownloader(clientExeName, url);
  m_waiting = true;
  cd->Work();
  Wait();
    
  // TODO Check download result. Display error msg to user if download failed.
}

void Updater::OnDownloadSuccess()
{
  // TODO Set success flag

  // Update version in config file once downloaded
  m_cf.Set(VERSION_KEY, m_version);
  m_cf.Save(CONFIG_FILE_NAME);
}

void Updater::OnServerResponse(const std::string& latest)
{
  Unwait();

std::cout << "Local version: " << m_version << ", latest version on server: " << latest << "\n";

  Strings strs[2] = { Split(m_version, '.'), Split(latest, '.') };
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
std::cout << "So I will download new client...\n";
  }
  else
  {
std::cout << "So no need to download new client.\n";
  }

  m_version = latest; // after comparison
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

void Updater::Start()
{
  File::SetRoot(GetSaveDir(APPLICATION), "/"); // all assets, downloaded files etc live in save dir

  m_downloadNewClient = false;

  // Compare versions
  std::string version;
  if (m_cf.Exists(VERSION_KEY))
  {
    m_version = m_cf.GetValue(VERSION_KEY);
std::cout << "Got current client version: " << m_version << "\n";
  }
  else
  {
    m_downloadNewClient = true;
std::cout << "I don't know the current client version, so will download.\n";
  }

std::cout << "Getting latest version info from server...\n";

  // Create downloader to get latest version on server
  std::string url = GetServer() + VERSION_SCRIPT;
  VersionChecker* v = new VersionChecker(url);
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
//  Launch(exe.c_str());

  execv(exe.c_str(), 0); //exe.c_str());
}

void Updater::Wait()
{
std::cout << "Waiting... ";
  // TODO Wait until signalled -- but watch out for race conditions --- could the signal alreay have happened ?!?
  while (m_waiting)
  {
//    std::cout << ".";
    // TODO Sleep
  }
  std::cout << "\n";
}

void Updater::Unwait()
{
  m_waiting = false;
}
}

