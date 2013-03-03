#include <AmjuAssert.h>
#include "ROConfig.h"

namespace Amju
{
static const char* RO_CONFIG_FILENAME = "roconfig.txt";

//void SaveROConfig()
//{
//  ROConfig()->Save(RO_CONFIG_FILENAME);
//}

ConfigFile* ROConfig()
{
  static ConfigFile* cf = 0;
  if (!cf)
  {
    cf = new ConfigFile;
	  if (!cf->Load(RO_CONFIG_FILENAME))
    {
      Assert(0);
    }

    // Duh, it's read only
	  //atexit(SaveROConfig);
  }
  return cf;
}

}
