// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <LoadVec2.h>
#include "GuiDecTranslate.h"

namespace Amju
{
const char* GuiDecTranslate::NAME = "translate";

bool GuiDecTranslate::Load(File* f)
{
  if (!LoadVec2(f, &m_localpos))
  {
    f->ReportError("Expected position for translate decorator");
    return false;
  }
  return GuiDecorator::Load(f);
}
}

