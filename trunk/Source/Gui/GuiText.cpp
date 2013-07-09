#include <AmjuFirst.h>
#include "GuiText.h"
#include "Font.h"
#include "DrawRect.h"
#include <Colour.h>
#include <StringUtils.h>
#include <Timer.h>
#include <TextToSpeech.h>
#include "DrawBorder.h"
#include <AmjuFinal.h>

//#define TEXT_RECT_DEBUG

namespace Amju
{
const char* GuiText::NAME = "gui-text";

// I.e. character at font size 1 takes up 1/20th of the screen height
const float GuiText::CHAR_HEIGHT_FOR_SIZE_1 = 0.1f; 

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
  m_first = 0;
  m_last = 0;
  m_caret = 0;
  m_selectedText = 0;
  m_scaleX = 1.0f;
}

void GuiText::SetScaleX(float scaleX)
{
  m_scaleX = scaleX;
}

float GuiText::GetScaleX() const
{
  return m_scaleX;
}

void GuiText::TextToSpeech()
{
  if (!m_text.empty())
  {
    Amju::TextToSpeech(m_text);
  }
}

void GuiText::SizeToText()
{
  if (IsMulti())
  {
    float longest = 0;
    for (unsigned int i = 0; i < m_lines.size(); i++)
    {
      float size = GetTextWidth(m_lines[i]) * m_textSize;
      if (size > longest)
      {
        longest = size;
      }
    }
    Vec2f size(longest, (float)(m_lines.size()) * CHAR_HEIGHT_FOR_SIZE_1 * m_textSize); 
    SetSize(size);
  }
  else
  {
    Vec2f size(GetTextWidth(m_text) * m_textSize, CHAR_HEIGHT_FOR_SIZE_1 * m_textSize); 
    SetSize(size);
  }
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
  RecalcFirstLast();
}

float GuiText::GetTextSize() const
{
  return m_textSize;
}

void GuiText::SetJust(Just j)
{
  m_just = j;
  RecalcFirstLast();
}

void GuiText::SetInverse(bool inv)
{
  m_inverse = inv;
}

void GuiText::SetDrawBg(bool drawBg)
{
  m_drawBg = drawBg;
}

void GuiText::SetFgCol(const Colour& col)
{
  m_fgCol = col;
}

void GuiText::SetBgCol(const Colour& col)
{
  m_bgCol = col;
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

void GuiText::SetFont(Font* font)
{
  m_font = font;
}

void GuiText::SetFont(const std::string& fontName)
{
  m_fontName = fontName;
  GetFont();
}

void GuiText::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  // TODO make this a flag ?
#ifdef TEXT_RECT_DEBUG
  DrawBorder(this, Colour(1, 1, 1, 1)); 
#endif // _DEBUG

  bool inverse = (m_inverse || IsSelected());

  if (m_drawBg)
  {
    DrawFilled(this, inverse ? m_fgCol : m_bgCol);
  }

  ReallyDraw();

  /*
  // Drop shadow version:
  // Only works if no BG. TODO Drop shadow dir, colours
  PushColour();
  ReallyDraw();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, 0.01f, 0);
  AmjuGL::SetColour(0, 0, 0, 1);
  ReallyDraw();
  AmjuGL::PopMatrix();
  PopColour();
  */
}

void GuiText::ReallyDraw()
{
  Font* font = GetFont();

  bool inverse = (m_inverse || IsSelected());

  float oldSize = font->GetSize();
  font->SetSize(m_textSize * oldSize);

  if (m_isMulti)
  {
    DrawMultiLine(inverse ? m_bgCol : m_fgCol, inverse ? m_fgCol : m_bgCol);
  }
  else
  {
    DrawSingleLine(m_first, m_last, inverse ? m_bgCol : m_fgCol, inverse ? m_fgCol : m_bgCol);

    // Draw selected - do draw BG
    if (m_caret != m_selectedText)
    {
      int left = std::min(m_caret, m_selectedText);
      int right = std::max(m_caret, m_selectedText);
    
      DrawSingleLine(left, right, inverse ? m_fgCol : m_bgCol, inverse ? m_bgCol : m_fgCol);
    }
  }

  font->SetSize(oldSize);
}

void GuiText::DrawMultiLine(const Colour& fg, const Colour& bg)
{
  Vec2f pos = GetCombinedPos();

  float y = pos.y - m_textSize * CHAR_HEIGHT_FOR_SIZE_1;
  // The extra fudge factor here is so we don't discard the last line
  //  when it fits ok, because it's a borderline case.
  float minY = pos.y - GetSize().y - 0.01f;

  int lines = m_lines.size();
  for (int i = m_topLine; i < lines; i++)
  {
    // TODO
    std::string str = m_lines[i];

    float x = GetCombinedPos().x;
    if (m_just == AMJU_JUST_RIGHT)
    {
      x += GetSize().x - GetTextWidth(str);
    }

    PushColour();
    AmjuGL::SetColour(fg);
    PrintLine(str, x, y); 
    PopColour();

    y -= m_textSize * CHAR_HEIGHT_FOR_SIZE_1;  
    // TODO Make text a window so lines will be clipped
    if (y < minY)
    {
      break;
    }
  }
}

void GuiText::RecalcFirstLast()
{
  GetFirstLast(0, &m_first, &m_last);
}

void GuiText::DrawSingleLine(int first, int last, const Colour& fg, const Colour& bg)
{
  // NB This is single line only, need another path for multi-line.
  // Decide on character range to draw
  std::string str = m_text.substr(first, last - first);
  //Trim(&str); ?

  Vec2f pos = GetCombinedPos();
  // Centre the text vertically
  float y = pos.y - GetSize().y * 0.5f - m_textSize * 0.5f * CHAR_HEIGHT_FOR_SIZE_1;

  float x = 0; // text x pos
  float xmin = 0; // bounding rect min and max x
  float xmax = 0;
  float startX = GetCombinedPos().x;

  Vec2f size = GetSize();
  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    xmin = (GetFont()->GetTextWidth(m_text.substr(m_first, first - m_first))) + startX;
    xmax = (GetFont()->GetTextWidth(m_text.substr(m_first, last - m_first))) + startX;
    x = xmin;
    break;

  case AMJU_JUST_RIGHT:
    x = GetCombinedPos().x + size.x - GetTextWidth(str);
    break;
  case AMJU_JUST_CENTRE:
    x = GetCombinedPos().x + 0.5f * (size.x - GetTextWidth(str));
    xmin = x;
    xmax = x + GetTextWidth(str) * GetTextSize();
    break;
  }

  // TODO Isn't this already done in ReallyDraw() ?
  if (m_drawBg)
  {
    PushColour();
    AmjuGL::SetColour(bg);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    Rect r; 
    r.Set(xmin, xmax, y, y + CHAR_HEIGHT_FOR_SIZE_1 * GetTextSize()); // TODO
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
  }

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

  PushColour();
  AmjuGL::SetColour(fg);
  PrintLine(str, x, y);
  PopColour();
}

void GuiText::PrintLine(const std::string& str, float x, float y)
{
  Font* font = GetFont();
  font->Print(x, y, str.c_str(), m_scaleX); 
}

void GuiText::GetFirstLast(int line, int* first, int* last)
{
  *first = 0;
  *last = m_text.size();
  Vec2f size = GetSize();

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    while (GetFont()->GetTextWidth(m_text.substr(0, *last)) > size.x)
    {
      (*last)--;
    }
    return;

  case AMJU_JUST_RIGHT:
    while (GetFont()->GetTextWidth(m_text.substr(*first)) > size.x)
    {
      (*first)++;
    }
    return;

  case AMJU_JUST_CENTRE:
    while (GetFont()->GetTextWidth(m_text.substr(*first, *last - *first)) > size.x)
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
    m_lines = WordWrap(m_text, GetSize().x / m_textSize, WidthFinder(this));
  }

  m_currentChar = 0;
  m_currentCharTime = 0;

  RecalcFirstLast();
}

bool GuiText::IsMulti() const
{
  return m_isMulti;
}

int GuiText::GetNumLines() const
{
  if (IsMulti())
  {
    return m_lines.size();
  }
  return 1; 
}

void GuiText::SetIsMulti(bool multi)
{
  if (multi == m_isMulti) return;

  m_isMulti = multi;
  SetText(m_text);
  RecalcFirstLast();
}

float GuiText::GetTextWidth(const std::string& text)
{
  float textWidth = GetFont()->GetTextWidth(text);
  return textWidth * m_scaleX;
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
  std::string text;
  if (!f->GetLocalisedString(&text))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }

  // Special code for empty string is $$$epty -- see File::GetDataLine() 
  //  - because an actual empty string will be ignored

  std::string s;
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
  if (size < 2)
  {
    f->ReportError("Unexpected font info: " + s);
  }
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
    else if (StringContains(s, "sx="))
    {
      m_scaleX = ToFloat(s.substr(4));
    }
    else
    {
      f->ReportError("Unexpected option: " + s);
      return false;
    }
  }

  SetText(text);

  RecalcFirstLast();
  return true;
}
}
