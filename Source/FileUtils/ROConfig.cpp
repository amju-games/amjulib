#include <AmjuFirst.h>
#include <AmjuAssert.h>
#include "ROConfig.h"
#include <AmjuFinal.h>

namespace Amju
{
static std::string s_ROConfigFilename = "roconfig.txt";
static const bool NO_ROOT = false;

//void SaveROConfig()
//{
//  ROConfig()->Save(RO_CONFIG_FILENAME);
//}

void SetROConfigFilename(const std::string& s)
{
  s_ROConfigFilename = s;
}

ConfigFile* ROConfig()
{
  static ConfigFile* cf = 0;
  if (!cf)
  {
    cf = new ConfigFile;

    if (!cf->Load(s_ROConfigFilename, NO_ROOT))
    {
      Assert(0);
    }

    // Duh, it's read only
    //atexit(SaveROConfig);
  }
  return cf;
}

void ReloadROConfig()
{
  ConfigFile* ro = ROConfig();
  ro->Clear();
  if (!ro->Load(s_ROConfigFilename, NO_ROOT))
  {
    Assert(0);
  }
}
}
