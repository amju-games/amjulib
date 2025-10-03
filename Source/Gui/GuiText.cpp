#include <AmjuFirst.h>
#include <algorithm>
#include <AmjuGL.h>
#include <Colour.h>
#include "DrawBorder.h"
#include "DrawRect.h"
#include "Font.h"
#include "GuiText.h"
#include <StringUtils.h>
#include <Timer.h>
#include <TextToSpeech.h>
#include <TriList.h>
#include <AmjuFinal.h>

//#define TEXT_RECT_DEBUG

namespace Amju
{
const char* GuiText::NAME = "gui-text";

// I.e. character at font size 1 takes up 1/20th of the screen height
const float GuiText::CHAR_HEIGHT_FOR_SIZE_1 = 0.1f;

GuiText::GuiText()
{
  m_fontName = "arial";
  m_fontPathFilename = "font2d/arial-font.font";
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
#ifdef AMJU_TEXT_TO_SPEECH
  if (!m_text.empty())
  {
    Amju::TextToSpeech(m_text);
  }
#endif
}

Vec2f GuiText::CalcSizeToText() const
{
  if (IsMulti())
  {
    float longest = 0;
    for (unsigned int i = 0; i < m_lines.size(); i++)
    {
      float w = GetTextWidth(m_lines[i]);
      longest = std::max(longest, w);
    }

    Vec2f size(
      longest,
      (float)(m_lines.size()) * CHAR_HEIGHT_FOR_SIZE_1 * m_textSize);

    return size;
  }
  else
  {
    Vec2f size(GetTextWidth(m_text), CHAR_HEIGHT_FOR_SIZE_1 * m_textSize);
    return size;
  }
}

void GuiText::SizeToText()
{
  SetSize(CalcSizeToText());
}

void GuiText::SetCharTime(float secs)
{
  m_charTime = secs;
  m_currentCharTime = 0;
  m_currentChar = 0;
}

void GuiText::SetFontSize(float textSize)
{
  m_textSize = textSize;
  RecalcFirstLast();
}

float GuiText::GetFontSize() const
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
    m_font = (Font*)TheResourceManager::Instance()->GetRes(m_fontPathFilename);
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
  m_fontPathFilename = fontName;
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
    DrawSingleLine(false, m_first, m_last, inverse ? m_bgCol : m_fgCol, inverse ? m_fgCol : m_bgCol);

    // Draw selected - do draw BG
    if (m_caret != m_selectedText)
    {
      int left = std::min(m_caret, m_selectedText);
      int right = std::max(m_caret, m_selectedText);
    
      DrawSingleLine(true, left, right, inverse ? m_fgCol : m_bgCol, inverse ? m_bgCol : m_fgCol);
    }
  }

  font->SetSize(oldSize);
}

void GuiText::DrawMultiLine(const Colour& fg, const Colour& bg)
{
  int lines = m_lines.size();
  
  bool rebuild = false;
  if (m_triLists.empty())
  {
    rebuild = true;
  }
  
  Font* font = GetFont();
  font->BindTexture();
  
  Vec2f pos = GetCombinedPos();

  float y = pos.y -m_textSize * CHAR_HEIGHT_FOR_SIZE_1;

  if (true /* vertically centred */)
  {
    float yoff = GetSize().y - lines * m_textSize * CHAR_HEIGHT_FOR_SIZE_1;
    y -= yoff * 0.5f;
  }

  // The extra fudge factor here is so we don't discard the last line
  //  when it fits ok, because it's a borderline case.
  float minY = pos.y - GetSize().y - 0.01f;

  PushColour();
  MultColour(fg);
  for (int i = 0; i < lines; i++)
  {
    std::string str = m_lines[i];

    float x = GetCombinedPos().x;
    if (m_just == AMJU_JUST_RIGHT)
    {
      x += GetSize().x - GetTextWidth(str) / m_textSize;
    }
    else if (m_just == AMJU_JUST_CENTRE)
    {
      float tw = GetTextWidth(str) / m_textSize;
      float sx = GetSize().x;
      x += (sx - tw) * 0.5f;
    }

    if (rebuild)
    {
      TriList* triList = font->MakeTriList(0, 0, str.c_str(), m_scaleX);
      m_triLists.push_back(triList);
      Assert((int)m_triLists.size() == i + 1);
    }
    
    Assert((int)m_triLists.size() > i);
    AmjuGL::PushMatrix();
    AmjuGL::Translate(x, y, 0);
    AmjuGL::Draw(m_triLists[i]);
    AmjuGL::PopMatrix();

    y -= m_textSize * CHAR_HEIGHT_FOR_SIZE_1;  
    // TODO Make text a window so lines will be clipped
    if (y < minY)
    {
      break;
    }
  }
  PopColour();
}

void GuiText::RecalcFirstLast()
{
  int first = m_first;
  int last = m_last;

  GetFirstLast(0, &m_first, &m_last);

  if (first != m_first || last != m_last)
  {
    m_triList = 0; // force rebuild
    m_triLists.clear();
  }
}

void GuiText::DrawSingleLine(
  bool selected, int first, int last, const Colour& fg, const Colour& bg)
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
    // divide by font size required to fix centering, but TODO: why??
    float w = GetTextWidth(str) / GetFontSize();
    x = GetCombinedPos().x + 0.5f * (size.x - w);
    xmin = x;
    xmax = x + w * GetFontSize();
    break;
  }

/*
  // TODO Isn't this already done in ReallyDraw() ?
  if (m_drawBg)
  {
    PushColour();
    AmjuGL::SetColour(bg);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    Rect r; 
    r.Set(xmin, xmax, y, y + CHAR_HEIGHT_FOR_SIZE_1 * GetFontSize()); // TODO
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
  }
*/

  if (m_charTime > 0)
  {
    // Gradually reveal text
    if (m_currentChar <= last)
    {
      str = str.substr(0, m_currentChar);

      m_currentCharTime += TheTimer::Instance()->GetDt();
      if (m_currentCharTime >= m_charTime)
      {
        m_currentChar++;
        m_currentCharTime = 0;
        m_triList = 0; // force rebuild
      }
    }
  }

  PushColour();
  MultColour(fg);
  
  Font* font = GetFont();
  if (selected && !m_triListSelection)
  {
    m_triListSelection = font->MakeTriList(0, 0, str.c_str(), m_scaleX);
  }
  else if (!selected && !m_triList)
  {
    m_triList = font->MakeTriList(0, 0, str.c_str(), m_scaleX);
  }
  
  font->BindTexture();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(x, y, 0);
  AmjuGL::Draw(selected ? m_triListSelection : m_triList);
  AmjuGL::PopMatrix();

  PopColour();
}

void GuiText::PrintLine(const std::string& str, float x, float y)
{
  Font* font = GetFont();
  if (!m_triList)
  {
    m_triList = font->MakeTriList(0, 0, str.c_str(), m_scaleX);
  }
  
  font->BindTexture();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(x, y, 0);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
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

std::string GuiText::GetText() const
{
  return m_text;
}

void GuiText::SetText(const std::string& escapedText)
{
  // Replace special codes for italics etc
  const char* ITALICS_ON = "\x01"; // match codes in Font
  const char* ITALICS_OFF = "\x02";

  std::string text;
  text = Replace(escapedText, "<i>", ITALICS_ON);
  text = Replace(text, "</i>", ITALICS_OFF);

  // Convert escape sequences
  text = ReplaceUtf8EscapedChars(text);

  if (text == m_text)
  {
    return;
  }

  m_triList = 0; // force rebuild
  m_triLists.clear();
  
  m_text = text;

  // Remove chars we can't display, which is anything with top bit set. This removes all 
  //  non-ascii unicode characters.
  // The way we display emojis etc is to use a native text view, e.g. GuiTextIos
  //m_text.erase(std::remove_if(m_text.begin(), m_text.end(), [](char c){ return (c & 0x80) != 0; }),
  //  m_text.end());

  if (m_isMulti)
  {
    // Replace the string "\n" with newline character
    m_text = Replace(m_text, "\\n", "\n");

    float availWidth = GetSize().x;
    m_lines = WordWrap(m_text, availWidth, WidthFinder(this));
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

float GuiText::GetTextWidth(const std::string& text) const
{
  float textWidth = const_cast<GuiText*>(this)->GetFont()->GetTextWidth(text);
  return textWidth * m_scaleX * m_textSize;
}

bool GuiText::Save(File* f)
{
  if (!f->Write(NAME))
  {
    return false;
  }
  if (!GuiElement::Save(f))
  {
    return false;
  }
  if (!SaveText(f))
  {
    return false;
  }
  return true;
}

bool GuiText::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  return LoadText(f);
}

bool GuiText::SaveText(File* f)
{
  if (!f->WriteComment("// \"" + m_text + "\""))
  {
    return false;
  }
  if (!f->Write(m_preLocalisedText))
  {
    return false;
  }
  // TODO
  std::string s = m_fontName + ", " + std::to_string(m_textSize);
  if (m_inverse)
  {
    s += ", inv";
  }
  if (m_isMulti)
  {
    s += ", multi";
  }
  // TODO bgCol, fgCol, scale, justification
  s += ", fgcol=" + ToHexString(m_fgCol);
  s += ", bgcol=" + ToHexString(m_bgCol);
  s += ", sx=" + ToString(m_scaleX);

  static const std::map<Just, std::string> JUST_MAP = { 
    {AMJU_JUST_LEFT, "left"},
    {AMJU_JUST_RIGHT, "right"},
    {AMJU_JUST_CENTRE, "centre"}
  };
  s += ", " + JUST_MAP.at(m_just);

  if (!f->Write(s))
  {
    return false;
  }

  return true;
}

bool GuiText::LoadText(File* f)
{
  std::string text;
  if (!f->GetLocalisedString(&text, &m_preLocalisedText))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }

  // Special code for empty string is $$$empty -- see File::GetDataLine() 
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

  m_fontName = strs[0];
  std::string fontPathFilename = "font2d/" + m_fontName + "-font.font";
  m_font = (Font*)TheResourceManager::Instance()->GetRes(fontPathFilename);
  if (!m_font)
  {
    f->ReportError("Failed to load font " + fontPathFilename);
    return false;
  }
  m_textSize = ToFloat(strs[1]);

  // Optional flags and attributes 
  for (int i = 2; i < size; i++)
  {
    std::string s = strs[i];
    Trim(&s);
    // Each attrib can be a single token or a pair of strings split by '='
    auto attrib = Split(s, '=');

    bool ok = (attrib.size() == 1 && HandleAttrib(attrib[0])) ||
      (attrib.size() == 2 && HandleAttrib(attrib[0], attrib[1]));

    if (!ok)
    {
      f->ReportError("Unexpected GUI text attrib: " + s);
      return false;
    }
  }

  SetText(text);

  RecalcFirstLast();
  return true;
}

bool GuiText::HandleAttrib(const std::string& s)
{
  if (s == "inv")
  {
    m_inverse = true;
    return true;
  }
  else if (s == "left")
  {
    m_just = AMJU_JUST_LEFT;
    return true;
  }
  else if (s == "right")
  {
    m_just = AMJU_JUST_RIGHT;
    return true;
  }
  else if (s == "centre")
  {
    m_just = AMJU_JUST_CENTRE;
    return true;
  }
  else if (s == "multi")
  {
    SetIsMulti(true);
    return true;
  }
  return false;
}

bool GuiText::HandleAttrib(const std::string& key, const std::string& value)
{
  if (key == "bgcol")
  {
    auto optionalColour = FromHexString(value);
    if (optionalColour)
    {
      m_bgCol = *optionalColour;
      m_drawBg = true;
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (key == "fgcol")
  {
    auto optionalColour = FromHexString(value);
    if (optionalColour)
    {
      m_fgCol = *optionalColour;
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (key == "sx")
  {
    m_scaleX = ToFloat(value);
    return true;
  }
  else if (key == "reveal")
  {
    // Arg is seconds to reveal each character in text
    float t = ToFloat(value);
    SetCharTime(t);
    return true;
  }
  return false;
}

const Colour& GuiText::GetFgCol() const
{
  return m_fgCol;
}

const Colour& GuiText::GetBgCol() const
{
  return m_bgCol;
}

}
