#ifndef SPECIAL_CONFIG_H_INCLUDED
#define SPECIAL_CONFIG_H_INCLUDED

#include <ConfigFile.h>

namespace Amju
{
class SpecialConfigFile : public ConfigFile
{
private:
  SpecialConfigFile() {}
  friend class Singleton<SpecialConfigFile>;
};
typedef Singleton<SpecialConfigFile> TheSpecialConfigFile;
}

#endif

