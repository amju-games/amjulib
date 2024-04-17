// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <StringUtils.h>
#include "GuiDecInclude.h"

namespace Amju
{
const char* GuiDecInclude::NAME = "include";

const std::string& GuiDecInclude::GetErrorStr() const
{
  return m_errorString;
}

bool GuiDecInclude::Save(File* f)
{
  if (!f->Write(GetTypeName()))
  {
    return false;
  }
  if (!f->Write(m_includeFilename))
  {
    return false;
  }
  return true;
}

bool GuiDecInclude::Load(File* f)
{
  if (!f->GetDataLine(&m_includeFilename))
  {
    f->ReportError("Expected include file name.");
    return false;
  }

  std::string path = GetFilePath(f->GetFileName());
  std::string pathAndChildFilename = path + "/" + m_includeFilename;

  File childFile;
  if (!childFile.OpenRead(pathAndChildFilename))
  {
    return false;
  }

  if (!GuiDecorator::Load(&childFile))
  {
    return false;
  }

  SetName("include-" + m_includeFilename);

  m_errorString = "included from " + f->GetFileName() + ", line " + ToString(f->GetLineNumber());

  return true;
}
}
