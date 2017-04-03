// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <AmjuGL.h>
#include "GuiDecRotate.h"

namespace Amju
{
const char* GuiDecRotate::NAME = "rotate";

bool GuiDecRotate::Load(File* f)
{
  if (!f->GetFloat(&m_angleDegs))
  {
    f->ReportError("Expected angle for rotate decorator");
    return false;
  }
  return GuiDecorator::Load(f);
}

void GuiDecRotate::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::RotateZ(m_angleDegs);
  GuiDecorator::Draw();
  AmjuGL::PopMatrix();
}
}

