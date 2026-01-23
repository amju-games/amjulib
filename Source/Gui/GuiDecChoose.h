// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecChoose *
// Composite/Decorator mutant which presents one child to descendants, 
//  chosen from all children.
// Child is chosen by the animValue passed into Animate.
class GuiDecChoose : public GuiDecorator
{
public:
  static const char* NAME;

  GuiDecChoose* Clone() override { return new GuiDecChoose(*this); }

  bool Load(File*) override;
  bool Save(File*) override;

  // Choose child to present to descendants
  void Animate(float animValue) override;

private:
  int m_chosenChild = 0;
  GuiElements m_allChildren;
};
}

