// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiComposite.h"

namespace Amju
{
// * GuiDecorator *
// Composite with one child. Subclasses change colour, position, etc of the child element.
class GuiDecorator : public GuiComposite
{
public:
  virtual bool Load(File* f) override { return LoadOneChild(f); }
  
  GuiElement* GetChild() { return GuiComposite::GetChild(0); }
};
}

