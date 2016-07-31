#pragma once

#include "GuiComposite.h"
#include "GuiRect.h"

namespace Amju
{
class GuiScrollBar : public GuiComposite
{
public:
  GuiScrollBar();

  // Assume size and pos set by the time you call this?
  void Init();

  // Set position between 0..1
  void SetScrollPos(float pos);

  // Set how much of the element can be seen - from 0..1
  // (1 means the entire element is visible, and so scrollbar is not needed.
  //  0 means NONE of the element can be seen, so prop should be > 0).
  void SetScrollProportionVisible(float prop);

  void SetIsVertical(bool);

private:
  // Set pos and size of inner rect
  void SetInnerRect();

private:
  RCPtr<GuiRect> m_bar;

  float m_scrollPos;
  float m_scrollPropVisible;
  bool m_isVertical;
};
}
