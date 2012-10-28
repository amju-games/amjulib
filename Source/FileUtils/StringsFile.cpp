#include "StringsFile.h"

namespace Amju
{
bool LoadStrings(const std::string& filename, Strings* result)
{
  File f;
  if (!f.OpenRead(filename, File::NO_VERSION))
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


