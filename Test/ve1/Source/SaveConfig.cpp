#include <iostream>
#include "SaveConfig.h"
#include "SpecialConfig.h"
#include <Directory.h>
#include <File.h>

namespace Amju
{
static const char* SPECIAL_FILE_NAME = "special.cfg";
static const char* CONFIG_FILE_NAME = "game.cfg";

void LoadConfig()
{
  // The "special" config file is used to set a custom server, environment, or Save Dir.
  // It's optional, intended only for testing, e.g. a test environment.
#ifdef MACOSX
  std::string specialFileName = GetProcessDir() + "/../../../" + SPECIAL_FILE_NAME;
#endif

#ifdef WIN32
  std::string specialFileName = GetProcessDir() + "/" + SPECIAL_FILE_NAME;
#endif

std::cout << "In LoadConfig().. attempting to load special config file: " << specialFileName << "\n";

  if (TheSpecialConfigFile::Instance()->Load(specialFileName, false))
  {
std::cout << "OK, loaded special config file!\n";
  }
  else
  {
std::cout << "No special config.\n";
  }

  // The above could fail, if no special config file - that's ok.

  std::string appname = GetAppName();

#ifdef MACOSX
  // Permissions issues make it best for every user to have a private Save Dir.
  // All Save Dirs are in ~/.mygame/ 

  std::string saveDir = GetDesktopDir() + "../.mygame/";
  MkDir(saveDir);
  saveDir += appname;
  MkDir(saveDir);
  saveDir += "/Data/";
  MkDir(saveDir);
  std::cout << "THIS IS THE SAVE DIR: " << saveDir << "\n";
  File::SetRoot(saveDir, "/");
  
#else
  File::SetRoot(GetSaveDir(appname) + "/Data/", "/");
#endif

  GameConfigFile* gcf = TheGameConfigFile::Instance();
  gcf->SetFilePath(CONFIG_FILE_NAME);
  gcf->Load();
  atexit(SaveConfig);
}

void SaveConfig()
{
  // called on exit
  TheGameConfigFile::Instance()->Save();
}
}

