// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecTranslate *
// Decorator which translates child
class GuiDecTranslate : public GuiDecorator
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;
  // Can't we just set the translation in Load?
//  virtual void Draw() override;

//private:
//  float m_angleDegs = 0;
};
}

