#include "GuiMenu.h"
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>

namespace Amju
{
GuiMenuItem::GuiMenuItem(const std::string& text)
{
  SetText(text);
}

void GuiMenuItem::Draw()
{
  // If selected, reverse colours
  GuiText::Draw();
}

GuiMenu::GuiMenu()
{
  m_selected = -1;
}

void GuiMenu::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_pos.x, m_pos.y, 0);
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    bool isSel = (m_selected == (int)i);
    GuiElement* item = m_items[i];
    Rect r(0, m_size.x, -item->GetSize().y, 0);
    PushColour();
    if (isSel)
    {
      MultColour(Colour(0, 0, 0, 1));
    }
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
    PushColour();
    if (!isSel)
    {
      MultColour(Colour(0, 0, 0, 1));
    }
    item->Draw();
    PopColour();
    AmjuGL::Translate(0, -item->GetSize().y, 0); 
  }
  AmjuGL::PopMatrix();
}

void GuiMenu::OnCursorEvent(const CursorEvent& ce)
{
  m_selected = -1;
  if (m_items.empty())
  {
    return;
  }

  float y = m_pos.y;
  // Check each item
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    GuiElement* item = m_items[i];
    // Make rect for item - width is max width
    Rect r(m_pos.x, m_pos.x + m_size.x, y - item->GetSize().y, y);

    if (r.IsPointIn(Vec2f(ce.x, ce.y)))
    {
      m_selected = i;
      return;
    }
    // Translate down
    y -= item->GetSize().y;
  }
}

void GuiMenu::AddItem(GuiMenuItem* pItem)
{
  m_items.push_back(pItem);

  // Adjust size of menu
  const Vec2f& size = pItem->GetSize();
  if (size.x > m_size.x)
  {
    m_size.x = size.x;
  }

  m_size.y += size.y;
}
}
