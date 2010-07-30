#include "LoadVec3.h"
#include "File.h"
#include "StringUtils.h"

namespace Amju
{
bool LoadVec3(File* f, Vec3f* v)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    Assert(0);
    return false;
  }
  Strings strs = Split(s, ',');
  Assert(strs.size() == 3);
  v->x = (float)atof(strs[0].c_str());
  v->y = (float)atof(strs[1].c_str());
  v->z = (float)atof(strs[2].c_str());
  return true;
}
}
