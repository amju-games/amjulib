#include "GuiText.h"
#include "Font.h"
#include "DrawRect.h"

namespace Amju
{
const char* GuiText::NAME = "gui-text";

GuiText::GuiText()
{
  m_fontName = "font2d/arial-font.font";
  m_just = AMJU_JUST_CENTRE;
  m_textWidth = 0;
}

Font* GuiText::GetFont()
{
  static Font* font = 
    (Font*)TheResourceManager::Instance()->GetRes(m_fontName);
  Assert(font);

  return font;
}

void GuiText::Draw()
{
#ifdef _DEBUG
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  Rect r = GetRect(this);
  DrawRect(r);
  AmjuGL::PopMatrix();
#endif // _DEBUG

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    GetFont()->Print(m_pos.x, m_pos.y, m_text.c_str());
    break;
  case AMJU_JUST_RIGHT:
    GetFont()->Print(m_pos.x + m_size.x - m_textWidth, m_pos.y, m_text.c_str());
    break;
  case AMJU_JUST_CENTRE:
    GetFont()->Print(m_pos.x + 0.5f * (m_size.x - m_textWidth), m_pos.y, m_text.c_str());
    break;
  default:
    Assert(0);
  }
}

void GuiText::SetText(const std::string& text)
{
  m_text = text;
  m_textWidth = GetFont()->GetTextWidth(text);
  //m_size.y = 0.08f; // Size of one line of text - TODO
  // TODO Multi lines; other GuiElement types
}

bool GuiText::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  std::string s;
  if (!f->GetLocalisedString(&s))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }
  SetText(s);

  // TODO Flags for multiline/word wrap
  // font name, font size, etc

  return true;
}
}
