#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <DrawRect.h>
#include <Rect.h>
#include "GuiElement.h"
#include "DrawBorder.h"
#include <AmjuFinal.h>

namespace Amju
{
void DrawBorder(GuiElement* gui, const Colour& col, float border)
{
  Rect r = GetRect(gui);
  float xmin = r.GetMin(0) - border;
  float xmax = r.GetMax(0) + border;
  float ymin = r.GetMin(1) - border;
  float ymax = r.GetMax(1) + border;
  r.Set(xmin, xmax, ymin, ymax);

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(col);
  DrawRect(r);
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

void DrawFilled(GuiElement* gui, const Colour& col, float border)
{
  Rect r = GetRect(gui);
  float xmin = r.GetMin(0) - border;
  float xmax = r.GetMax(0) + border;
  float ymin = r.GetMin(1) - border;
  float ymax = r.GetMax(1) + border;
  r.Set(xmin, xmax, ymin, ymax);

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(col);
  DrawSolidRect(r);
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}
}
