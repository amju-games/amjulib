#include <AmjuFirst.h>
#include "LoadRect.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
bool LoadRect(File* f, Rect* r)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  std::vector<std::string> strs = Split(s, ',');
  if (strs.size() != 4)
  {
    f->ReportError("Not a Rect");
    return false;
  }

  r->Set((float)atof(strs[0].c_str()), (float)atof(strs[1].c_str()),
    (float)atof(strs[2].c_str()), (float)atof(strs[3].c_str()));

  return true;
}
}