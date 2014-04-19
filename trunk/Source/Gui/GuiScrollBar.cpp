#include "GuiScrollBar.h"

namespace Amju
{
GuiScrollBar::GuiScrollBar()
{
  m_scrollPos = 0;
  m_scrollPropVisible = 0.5f; // ?
  m_isVertical = true;
}

void GuiScrollBar::Init()
{
  // Background colour
  GuiRect* rect = new GuiRect;
  float g = 0.9f; // grey 
  rect->SetColour(Colour(g, g, g, 1));
  rect->SetSize(GetSize()); 
  AddChild(rect);

  // Foreground bar
  m_bar = new GuiRect;
  g = 0.1f; 
  rect->SetColour(Colour(g, g, g, 1));
  AddChild(rect);

  SetInnerRect();
}

void GuiScrollBar::SetScrollPos(float pos)
{
  m_scrollPos = pos;
  SetInnerRect();
}

void GuiScrollBar::SetScrollProportionVisible(float prop)
{
  m_scrollPropVisible = prop;
  SetInnerRect();
}

void GuiScrollBar::SetIsVertical(bool vertical)
{
  m_isVertical = vertical;
  SetInnerRect();
}

void GuiScrollBar::SetInnerRect()
{
  Assert(m_bar);
  // Set the position and size of m_bar
  if (m_isVertical)
  {
    Vec2f& pos = GetLocalPos(); // pos.y is top
    Vec2f& size = GetSize();
    float w = size.x;
    float h = m_scrollPropVisible * size.y;
//    float x = pos.x;
//    float y = pos.y - m_scrollPos * size.y;
    m_bar->SetSize(Vec2f(w, 0.1f)); //h));
    m_bar->SetLocalPos(Vec2f(0, -m_scrollPos * size.y));
  }
  else
  {
  }
}

}
