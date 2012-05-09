#include "SaveConfig.h"
#include <ConfigFile.h>
#include <Directory.h>

namespace Amju
{
static const char* CONFIG_FILE_NAME = "game.cfg";

std::string GetConfigFilename()
{
  // File game.cfg in same dir as executable

  static std::string configFileName = GetProcessDir() + "/" + CONFIG_FILE_NAME;
  return configFileName;
}

void LoadConfig()
{
  TheGameConfigFile::Instance()->Load(GetConfigFilename(), false);
  atexit(SaveConfig);
}

void SaveConfig()
{
  // called on exit
  TheGameConfigFile::Instance()->Save(GetConfigFilename(), false);
}
}

