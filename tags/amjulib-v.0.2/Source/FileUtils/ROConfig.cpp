#include <AmjuFirst.h>
#include <AmjuAssert.h>
#include "ROConfig.h"
#include <AmjuFinal.h>

namespace Amju
{
static std::string s_ROConfigFilename = "roconfig.txt";

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
    bool NO_ROOT = false;
    if (!cf->Load(s_ROConfigFilename, NO_ROOT))
    {
      Assert(0);
    }

    // Duh, it's read only
    //atexit(SaveROConfig);
  }
  return cf;
}

}
