#include "GuiText.h"
#include "Font.h"
#include "DrawRect.h"
#include "Colour.h"
#include <StringUtils.h>

#define TEXT_RECT_DEBUG

namespace Amju
{
const char* GuiText::NAME = "gui-text";

GuiText::GuiText()
{
  m_fontName = "font2d/arial-font.font";
  m_just = AMJU_JUST_CENTRE;
  m_inverse = false;
  m_drawBg = false;
  m_textSize = 1.0f;
}

void GuiText::SetTextSize(float textSize)
{
  m_textSize = textSize;
}

void GuiText::SetJust(Just j)
{
  m_just = j;
}

void GuiText::SetInverse(bool inv)
{
  m_inverse = inv;
}

void GuiText::SetDrawBg(bool drawBg)
{
  m_drawBg = drawBg;
}

Font* GuiText::GetFont()
{
  if (!m_font)
  {
    m_font = (Font*)TheResourceManager::Instance()->GetRes(m_fontName);
  }

  Assert(m_font);
  return m_font;
}

void GuiText::Draw()
{
  if (!IsVisible())
  {
    return;
  }

#ifdef TEXT_RECT_DEBUG
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  Rect r = GetRect(this);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawRect(r);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::PopMatrix();
#endif // _DEBUG

  Font* font = GetFont();

  PushColour();
  static const Colour WHITE(1, 1, 1, 1);
  static const Colour BLACK(0, 0, 0, 1);

  if (m_drawBg)
  {
    // TODO Mult colour
    AmjuGL::SetColour(m_inverse ? BLACK : WHITE);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    Rect r = GetRect(this);
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

    // Set text colour to contrast with BG, but only if BG is drawn
    AmjuGL::SetColour(m_inverse ? WHITE: BLACK);
  }

  // Centre the text vertically
  //float ysize = m_size.y - 0.1f; // TODO Why 0.1, depends on font size ?
  float y = m_pos.y - m_size.y * 0.5f - m_textSize * 0.05f;

  float oldSize = font->GetSize();
  font->SetSize(m_textSize * oldSize);

  // NB This is single line only, need another path for multi-line.
  // Decide on character range to draw
  int first, last;
  GetFirstLast(0, &first, &last);
  last -= first;
  std::string str = m_text.substr(first, last);
  Trim(&str);

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    font->Print(m_pos.x, y, str.c_str());
    break;
  case AMJU_JUST_RIGHT:
    font->Print(m_pos.x + m_size.x - GetTextWidth(str), y, str.c_str());
    break;
  case AMJU_JUST_CENTRE:
    font->Print(m_pos.x + 0.5f * (m_size.x - GetTextWidth(str)), y, str.c_str());
    break;
  default:
    Assert(0);
  }
  font->SetSize(oldSize);

  PopColour();
}

void GuiText::GetFirstLast(int line, int* first, int* last)
{
  *first = 0;
  *last = m_text.size();

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    while (GetFont()->GetTextWidth(m_text.substr(0, *last)) > m_size.x)
    {
      (*last)--;
    }
    return;

  case AMJU_JUST_RIGHT:
    while (GetFont()->GetTextWidth(m_text.substr(*first)) > m_size.x)
    {
      (*first)++;
    }
    return;

  case AMJU_JUST_CENTRE:
    while (GetFont()->GetTextWidth(m_text.substr(*first, *last - *first)) > m_size.x)
    {
      (*first)++;
      (*last)--;
    }
    return;

  default:
    Assert(0);
  }
}

//int GuiText::GetLast(int line)
//{
//  int first = 0;
//  int last = m_text.size();
//
//  switch (m_just)
//  {
//  case AMJU_JUST_LEFT:
//    while (GetFont()->GetTextWidth(m_text.substr(0, last)) > m_size.x)
//    {
//      last--;
//    }
//    return last;
//
//  case AMJU_JUST_RIGHT:
//    return last;
//
//  case AMJU_JUST_CENTRE:
//    while (GetFont()->GetTextWidth(m_text.substr(first, last - first)) > m_size.x)
//    {
//      first++;
//      last--;
//    }
//    return last;
//
//  default:
//    Assert(0);
//  }
//
//  return 0;
//}

void GuiText::SetText(const std::string& text)
{
  m_text = text;
}

float GuiText::GetTextWidth(const std::string& text)
{
  float textWidth = GetFont()->GetTextWidth(text);
  return textWidth;
  //m_size.y = 0.08f; // Size of one line of text - TODO
  // TODO Multi lines; other GuiElement types
}

bool GuiText::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  return LoadText(f);
}

bool GuiText::LoadText(File* f)
{
  std::string s;
  if (!f->GetLocalisedString(&s))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }
  SetText(s);

  // TODO Flags for multiline/word wrap
  // font name, font size, etc
  // Get font name and size
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected font info");
    return false;
  }
  Strings strs = Split(s, ',');
  Assert(strs.size() == 2);
  std::string fontName = "font2d/" + strs[0] + "-font.font";
  m_font = (Font*)TheResourceManager::Instance()->GetRes(fontName);
  m_textSize = ToFloat(strs[1]);

  return true;
}
}
