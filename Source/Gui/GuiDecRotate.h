// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecRotate *
// Decorator which rotates child
class GuiDecRotate : public GuiDecorator
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;
  virtual void Draw() override;

private:
  float m_angleDegs = 0;
};
}

