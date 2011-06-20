#include "GuiText.h"
#include "Font.h"
#include "DrawRect.h"
#include "Colour.h"

//#define TEXT_RECT_DEBUG

namespace Amju
{
const char* GuiText::NAME = "gui-text";

GuiText::GuiText()
{
  m_fontName = "font2d/arial-font.font";
  m_just = AMJU_JUST_CENTRE;
  m_textWidth = 0;
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
  float y = m_pos.y - m_size.y * 0.5f - 0.05f;

  float oldSize = font->GetSize();
  font->SetSize(m_textSize * oldSize);
  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    font->Print(m_pos.x, y, m_text.c_str());
    break;
  case AMJU_JUST_RIGHT:
    font->Print(m_pos.x + m_size.x - m_textWidth, y, m_text.c_str());
    break;
  case AMJU_JUST_CENTRE:
    font->Print(m_pos.x + 0.5f * (m_size.x - m_textWidth), y, m_text.c_str());
    break;
  default:
    Assert(0);
  }
  font->SetSize(oldSize);

  PopColour();
}

void GuiText::SetText(const std::string& text)
{
  m_text = text;
  m_textWidth = GetFont()->GetTextWidth(text);
  m_textWidth *= m_textSize;

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
