#include <AmjuFirst.h>
#include "LoadVec2.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
bool ToVec2(const std::string& s, Vec2i* v)
{
  std::vector<std::string> strs = Split(s, ',');
  if (strs.size() != 2)
  {
    return false;
  }

  v->x = atoi(strs[0].c_str());
  v->y = atoi(strs[1].c_str());
  return true;
}

bool ToVec2(const std::string& s, Vec2f* v)
{
  std::vector<std::string> strs = Split(s, ',');
  if (strs.size() != 2)
  {
    return false;
  }

  v->x = (float)atof(strs[0].c_str());
  v->y = (float)atof(strs[1].c_str());
  return true;
}

bool LoadVec2(File* f, Vec2f* v)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }

  return ToVec2(s, v);
}

bool LoadVec2(File* f, Vec2i* v)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  std::vector<std::string> strs = Split(s, ',');
  if (strs.size() != 2)
  {
    f->ReportError("Not a Vec2");
    return false;
  }

  v->x = atoi(strs[0].c_str());
  v->y = atoi(strs[1].c_str());
  return true;
}
}
