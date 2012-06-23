#include <iostream>
#include "SpecialConfig.h"

namespace Amju
{
static const char* DEFAULT_SERVER = "www.amju.com";
static const char* DEFAULT_ENV = "ve1";

static const char* SERVER_KEY = "server";
static const char* ENV_KEY = "env";

static const char* APP_NAME = "ve1"; // default name for Save Dir
static const char* APP_NAME_CONFIG_KEY = "appname"; // the key in the special config file

std::string GetServer()
{
  static ConfigFile* config = TheSpecialConfigFile::Instance();
  std::string server = DEFAULT_SERVER;
  if (config->Exists(SERVER_KEY))
  {
    server = config->GetValue(SERVER_KEY);
  }
  return server;
}

std::string GetEnv()
{
  static ConfigFile* config = TheSpecialConfigFile::Instance();
  std::string env = DEFAULT_ENV;
  if (config->Exists(ENV_KEY))
  {
    env = config->GetValue(ENV_KEY);
  }
  return env;
}

std::string GetAppName()
{
  static ConfigFile* config = TheSpecialConfigFile::Instance();
  std::string appname = APP_NAME;
  if (config->Exists(APP_NAME_CONFIG_KEY))
  {
    appname = TheSpecialConfigFile::Instance()->GetValue(APP_NAME_CONFIG_KEY);
  }
  return appname;
}
}

