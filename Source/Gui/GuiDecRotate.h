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
  virtual void Animate(float animValue) override;

private:
  float m_angle[2];
  float m_interpAngle = 0;
};
}

