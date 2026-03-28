#include <AmjuFirst.h>
#include "LoadVec3.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
bool SaveVec3(File* f, const Vec3f& v)
{
  if (f->IsBinary())
  {
    return f->WriteFloat(v.x) && f->WriteFloat(v.y) && f->WriteFloat(v.z);
  }
  else
  {
    std::string str = ToString(v.x) + ", " + ToString(v.y) + ", " + ToString(v.z);
    return (f->Write(str));
  }
}

bool LoadVec3OptionalZ(File* f, Vec3f* v, float defaultZ)
{
  // For a binary file, the z coord is non optional.
  if (f->IsBinary())
  {
    return f->GetFloat(&(v->x)) && 
           f->GetFloat(&(v->y)) && 
           f->GetFloat(&(v->z));
  }

  std::string line;
  if (!f->GetDataLine(&line))
  {
    f->ReportError("Expected vec2/3.");
    return false;
  }

  auto vec = SplitIntoVec<float>(line, ',');
  
  if (vec.size() == 2)
  {
    *v = Vec3f(vec[0], vec[1], defaultZ);
    return true;
  }

  if (vec.size() == 3)
  {
    *v = Vec3f(vec[0], vec[1], vec[2]);
    return true;
  }

  f->ReportError("Unexpected format.");
  return false;
}

bool LoadVec3(File* f, Vec3f* v)
{
  if (f->IsBinary())
  {
    return f->GetFloat(&(v->x)) && f->GetFloat(&(v->y)) && f->GetFloat(&(v->z));
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  Strings strs = Split(s, ',');
  if (strs.size() != 3)
  {
    f->ReportError("Not a Vec3");
    return false;
  }

  v->x = (float)atof(strs[0].c_str());
  v->y = (float)atof(strs[1].c_str());
  v->z = (float)atof(strs[2].c_str());
  return true;
}
}
