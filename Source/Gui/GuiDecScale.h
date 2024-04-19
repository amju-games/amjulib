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
  std::string GetTypeName() const override { return NAME; }

  GuiDecScale* Clone() override { return new GuiDecScale(*this); }

  bool Load(File*) override;
  bool Save(File*) override;
  void Draw() override;
  void Animate(float animValue) override;

  const Vec2f& GetPivot() const;
  Vec2f GetScale() const override;

  void SetSecondScale(const Vec2f& scale); // For animation, we interpolate between two scale factors.
  void SetPivot(const Vec2f& pivot);

private:
  Vec2f m_secondScale;
  Vec2f m_interpScale;
  Vec2f m_pivot;
  Vec2f m_origChildSize;
};
}
