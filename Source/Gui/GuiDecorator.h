// * Amjulib *
// (c) Copyright 2000-2017 Juliet Colman

#pragma once

#include "GuiComposite.h"

namespace Amju
{
// * GuiDecorator *
// Composite with one child. Subclasses change colour, position, etc of the child element.
class GuiDecorator : public GuiComposite
{
public:
  bool Load(File* f) override;
  bool Save(File* f) override;

  GuiElement* GetChild();
};
}

