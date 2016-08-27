#include <AmjuFirst.h>
#include "LoadMatrix.h"
#include "Matrix.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

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
  
  if (strs.size() != 16)
  {
    f->ReportError("Expected line of 16 matrix elements (or \"I\") here.");
    return false;
  }
  
  for (int i = 0; i < 16; i++)
  {
    (*m)[i] = (float)atof(strs[i].c_str());
  }
  return true;
}
}
