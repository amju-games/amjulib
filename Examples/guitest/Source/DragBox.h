#pragma once

#include <GuiElement.h>
#include <GuiRect.h>

namespace Amju
{
class DragBox : public GuiRect
{
public:
  DragBox();
  void Draw() override;

private:
  GuiRect m_corners[4];
};
}


