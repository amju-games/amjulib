#include "GuiText.h"
#include "Font.h"
#include "DrawRect.h"
#include "Colour.h"
#include <StringUtils.h>
#include <Timer.h>

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
  static const Colour WHITE(1, 1, 1, 1);
  static const Colour BLACK(0, 0, 0, 1);
  m_bgCol = WHITE;
  m_fgCol = BLACK;
  SetCharTime(0);
  m_isMulti = false;
  m_topLine = 0;
}

void GuiText::SizeToText()
{
  m_size.x = GetTextWidth(m_text) * m_textSize;
  m_size.y = 0.1f * m_textSize; // font size of 1 needs height 0.1
}

void GuiText::SetCharTime(float secs)
{
  m_charTime = secs;
  m_currentCharTime = 0;
  m_currentChar = 0;
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
  Rect r = GetRect(this);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawRect(r);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
#endif // _DEBUG

  Font* font = GetFont();

  PushColour();

  bool inverse = (m_inverse || IsSelected());
  if (m_drawBg)
  {
    // TODO Mult colour ?
    AmjuGL::SetColour(inverse ? m_fgCol : m_bgCol);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    Rect r = GetRect(this);
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

    // Set text colour to contrast with BG, but only if BG is drawn
    AmjuGL::SetColour(inverse ? m_bgCol: m_fgCol);
  }

  float oldSize = font->GetSize();
  font->SetSize(m_textSize * oldSize);

  if (m_isMulti)
  {
    DrawMultiLine();
  }
  else
  {
    DrawSingleLine();
  }

  font->SetSize(oldSize);
  PopColour();
}

void GuiText::DrawMultiLine()
{
  float y = m_pos.y - m_textSize * 0.1f;
  float minY = m_pos.y - m_size.y;

  int lines = m_lines.size();
  for (int i = m_topLine; i < lines; i++)
  {
    // TODO
    std::string str = m_lines[i];

    PrintLine(str, y); 
    y -= m_textSize * 0.1f; // 0.1 is height of text at size 1.0
    if (y < minY)
    {
      break;
    }
  }
}

void GuiText::DrawSingleLine()
{
  // Centre the text vertically
  // TODO Why 0.1, depends on font size ?
  float y = m_pos.y - m_size.y * 0.5f - m_textSize * 0.05f;

  // NB This is single line only, need another path for multi-line.
  // Decide on character range to draw
  int first, last;
  GetFirstLast(0, &first, &last);
  last -= first; // convert from index to number of chars to draw
  std::string str = m_text.substr(first, last);
  Trim(&str);

  if (m_charTime > 0)
  {
    // Gradually reveal text
    if (m_currentChar < last)
    {
      str = str.substr(0, m_currentChar);

      m_currentCharTime += TheTimer::Instance()->GetDt();
      if (m_currentCharTime >= m_charTime)
      {
        m_currentChar++;
        m_currentCharTime = 0;
      }
    }
  }

  PrintLine(str, y);
}

void GuiText::PrintLine(const std::string& str, float y)
{
  Font* font = GetFont();
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

struct WidthFinder
{
  WidthFinder(GuiText* g) : m_guiText(g) {}
  float operator()(const std::string& s)
  {
    return m_guiText->GetTextWidth(s);
  }
  GuiText* m_guiText;
};

const std::string& GuiText::GetText() const
{
  return m_text;
}

void GuiText::SetText(const std::string& text)
{
  m_text = text;

  if (m_isMulti)
  {
    m_lines = WordWrap(m_text, m_size.x / m_textSize, WidthFinder(this));
  }

  m_currentChar = 0;
  m_currentCharTime = 0;
}

void GuiText::SetIsMulti(bool multi)
{
  m_isMulti = multi;
  SetText(m_text);
}

float GuiText::GetTextWidth(const std::string& text)
{
  float textWidth = GetFont()->GetTextWidth(text);
  return textWidth;
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
  int size = strs.size();
  Assert(size >= 2);
  std::string fontName = "font2d/" + strs[0] + "-font.font";
  m_font = (Font*)TheResourceManager::Instance()->GetRes(fontName);
  m_textSize = ToFloat(strs[1]);

  // TODO optional flags etc
  for (int i = 2; i < size; i++)
  {
    std::string s = strs[i];
    Trim(&s);

    if (s == "inv")
    {
      m_inverse = true;
    }
    else if (s == "left")
    {
      m_just = AMJU_JUST_LEFT;
    }
    else if (s == "right")
    {
      m_just = AMJU_JUST_RIGHT;
    }
    else if (s == "centre")
    {
      m_just = AMJU_JUST_CENTRE;
    }
    else if (s == "multi")
    {
      SetIsMulti(true);
    }
    else if (StringContains(s, "bgcol="))
    {
      std::string c = s.substr(6); // colour code should start at 6th char
      m_bgCol = FromHexString(c);
      m_drawBg = true;
    }
    else if (StringContains(s, "fgcol="))
    {
      std::string c = s.substr(6); // colour code should start at 6th char
      m_fgCol = FromHexString(c);
    }
    else
    {
      f->ReportError("Unexpected option: " + s);
      return false;
    }
  }

  return true;
}
}
