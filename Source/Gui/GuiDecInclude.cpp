// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <StringUtils.h>
#include "GuiDecInclude.h"

namespace Amju
{
const char* GuiDecInclude::NAME = "include";

bool GuiDecInclude::Load(File* f)
{
  std::string childFilename;
  if (!f->GetDataLine(&childFilename))
  {
    f->ReportError("Expected child file name.");
    return false;
  }

  std::string path = GetFilePath(f->GetName());
  childFilename = path + "/" + childFilename;

  File childFile;
  if (!childFile.OpenRead(childFilename))
  {
    return false;
  }

  return LoadOneChild(&childFile);
}
}
