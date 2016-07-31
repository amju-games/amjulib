#pragma once

#include "GuiButton.h"

namespace Amju
{
class GuiSlider : public GuiButton
{
public:
  static const char* NAME;

  GuiSlider();

  virtual bool Load(File*) override;
  virtual void Draw() override;
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool IsEnabled() const override;

  void SetXRange(float minX, float maxX) { m_minX = minX; m_maxX = maxX; }

  GuiImage* GetBgImage() { return &m_bg; }

protected:
  float m_minX;
  float m_maxX;

  GuiImage m_bg;
};
}


