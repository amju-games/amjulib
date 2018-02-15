// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecScale *
// Decorator which scales child. This can be animated: we can specify 2 scales, and
//  use an animation decorator to control interpolation between them.
// If two scales are given, separate them with semicolon.
class GuiDecScale : public GuiDecorator
{
public:
  static const char* NAME;
  virtual bool Load(File*) override;
  virtual void Draw() override;
  virtual void Animate(float animValue) override;

  void SetScale(const Vec2f& scale, int zeroOrOne = 0);
  void SetPivot(const Vec2f& pivot);

private:
  Vec2f m_scale[2];
  Vec2f m_interpScale;
  Vec2f m_pivot;
  Vec2f m_origChildSize;
};
}
