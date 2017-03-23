// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecInclude *
// Decorator which loads child from a file. This lets us reuse GUIs in files.
class GuiDecInclude : public GuiDecorator 
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;
};
}

