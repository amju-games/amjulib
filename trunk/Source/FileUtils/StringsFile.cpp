#include <AmjuFirst.h>
#include "StringsFile.h"
#include <AmjuFinal.h>

namespace Amju
{
bool LoadStrings(const std::string& filename, Strings* result)
{
  File f(File::NO_VERSION);
  if (!f.OpenRead(filename))
  {
    return false;
  }
  std::string str;
  while (f.GetDataLine(&str))
  {
    result->push_back(str);
  }
  return true;
}

bool SaveStrings(const std::string& filename, const Strings& strs)
{
  return false; // TODO
}

}


