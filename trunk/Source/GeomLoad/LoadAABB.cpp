#include <AmjuFirst.h>
#include "LoadAABB.h"
#include <File.h>
#include <AABB.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

namespace Amju
{
bool LoadAABB(File* f, AABB* aabb)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    Assert(0);
    return false;
  }
  Strings strs = Split(s, ',');
  Assert(strs.size() == 6);
  aabb->Set(
    ToFloat(strs[0]), 
    ToFloat(strs[1]),
    ToFloat(strs[2]),
    ToFloat(strs[3]),
    ToFloat(strs[4]),
    ToFloat(strs[5]));

  return true;

}
}
