// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecRotate *
// Decorator which rotates child. One or 2 angles can be specified - if
//  two, animation interpolates between the two.
// Angles are in degrees.
class GuiDecRotate : public GuiDecorator
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;
  virtual void Draw() override;
  virtual void Animate(float animValue) override;

  void SetRotation(float angle, int zeroOrOne = 0);
  void SetPivot(const Vec2f& pivot);

private:
  float m_angle[2];
  float m_interpAngle = 0;
  Vec2f m_pivot;
};
}

