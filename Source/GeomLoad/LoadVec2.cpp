#include "LoadVec2.h"
#include "File.h"
#include "StringUtils.h"

namespace Amju
{
bool LoadVec2(File* f, Vec2f* v)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    Assert(0);
    return false;
  }
  std::vector<std::string> strs = Split(s, ',');
  Assert(strs.size() == 2);
  v->x = (float)atof(strs[0].c_str());
  v->y = (float)atof(strs[1].c_str());
  return true;
}
}
