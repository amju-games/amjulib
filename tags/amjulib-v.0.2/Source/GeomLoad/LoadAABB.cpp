#include <AmjuFirst.h>
#include "LoadAABB.h"
#include <File.h>
#include <AABB.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

namespace Amju
{
bool SaveAABB(File* f, const AABB& aabb)
{
  if (f->IsBinary())
  {
    return f->WriteFloat(aabb.GetMin(0)) &&
      f->WriteFloat(aabb.GetMax(0)) &&
      f->WriteFloat(aabb.GetMin(1)) &&
      f->WriteFloat(aabb.GetMax(1)) &&
      f->WriteFloat(aabb.GetMin(2)) &&
      f->WriteFloat(aabb.GetMax(2));
  }
  std::string s = ToString(aabb.GetMin(0)) + ", " +
    ToString(aabb.GetMax(0)) + ", " +
    ToString(aabb.GetMin(1)) + ", " +
    ToString(aabb.GetMax(1)) + ", " +
    ToString(aabb.GetMin(2)) + ", " +
    ToString(aabb.GetMax(2));

  return f->WriteComment("// AABB") && f->Write(s);
}

bool LoadAABB(File* f, AABB* aabb)
{
  if (f->IsBinary())
  {
    float a[6];
    if (!f->GetFloat(&a[0]) || 
      !f->GetFloat(&a[1]) ||
      !f->GetFloat(&a[2]) ||
      !f->GetFloat(&a[3]) ||
      !f->GetFloat(&a[4]) ||
      !f->GetFloat(&a[5]))
    {
      return false; 
    }
    aabb->Set(a[0], a[1], a[2], a[3], a[4], a[5]);
    return true;
  }

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
