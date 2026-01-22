// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include <AmjuRand.h>
#include "GuiDecAnimRandom.h"

namespace Amju
{
const char* GuiDecAnimRandom::NAME = "random";

void GuiDecAnimRandom::Update() 
{
 if (!IsVisible())
  {
    return;
  }

  if (IsPaused())
  {
    return;
  }

  GetChild()->Animate(m_random);
  GetChild()->Update();
}

bool GuiDecAnimRandom::Load(File* f) 
{
  // Animations need names so we can get to them in code
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected animation name");
    return false;
  }

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  m_random = Amju::Rnd(0, 1);

  return true;
}

bool GuiDecAnimRandom::Save(File* f)
{
  if (!SaveTypeAndName(f))
  {
    return false;
  }
  return GetChild()->Save(f);
}

void GuiDecAnimRandom::Animate(float animValue)
{
  // Nothing to do here. animValue doesn't affect m_random.
}
}

