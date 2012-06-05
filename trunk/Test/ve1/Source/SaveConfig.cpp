#include <iostream>
#include "SaveConfig.h"
#include "SpecialConfig.h"
#include <Directory.h>
#include <File.h>

namespace Amju
{
static const char* SPECIAL_FILE_NAME = "special.cfg";
static const char* CONFIG_FILE_NAME = "game.cfg";
static const char* APP_NAME = "ve1"; // default name for Save Dir
static const char* APP_NAME_CONFIG_KEY = "appname"; // the key in the special config file

void LoadConfig()
{
  // The "special" config file is used to set a custom server, environment, or Save Dir.
  // It's optional, intended only for testing, e.g. a test environment.
  std::string specialFileName = GetProcessDir() + "/" + SPECIAL_FILE_NAME;

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

  std::string appname = APP_NAME;
  if (TheSpecialConfigFile::Instance()->Exists(APP_NAME_CONFIG_KEY))
  {
    appname = TheSpecialConfigFile::Instance()->GetValue(APP_NAME_CONFIG_KEY);
std::cout << "Found app name in special config: " << appname << "\n";
  }
  else
  {
std::cout << "No key " << APP_NAME_CONFIG_KEY << " in special config.\n";
  }

  File::SetRoot(GetSaveDir(appname) + "/Data/", "/");

  TheGameConfigFile::Instance()->Load(CONFIG_FILE_NAME);
  atexit(SaveConfig);
}

void SaveConfig()
{
  // called on exit
  TheGameConfigFile::Instance()->Save(CONFIG_FILE_NAME);
}
}

