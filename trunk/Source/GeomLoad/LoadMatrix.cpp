#include "LoadMatrix.h"
#include "Matrix.h"
#include "File.h"
#include "StringUtils.h"

namespace Amju
{
bool LoadMatrix(File* f, Matrix* m)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  if (s == "I")
  {
    m->SetIdentity();
    return true;
  }
  std::vector<std::string> strs = Split(s, ',');
  Assert(strs.size() == 16);
  for (int i = 0; i < 16; i++)
  {
    (*m)[i] = (float)atof(strs[i].c_str());
  }
  return true;
}
}
