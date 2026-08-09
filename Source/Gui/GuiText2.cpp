#include "GuiText2.h"

namespace Amju
{
const char* GuiText2::NAME = "text2";

void GuiText2::Draw() 
{
  if (!IsVisible())
  {
    return;
  }

  AddToBatch(this);

  // Store current state of transform and colour, which we use later when
  //  we draw the batch of all tris using the current texture.
  const Vec2f& pos = GetCombinedPos();
  const Vec2f& size = GetSize() * GetCombinedScale();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x, pos.y, 0); 
  AmjuGL::Scale(size.x, size.y, 1); 
  m_combinedTransform.ModelView();
  m_combinedColour = AmjuGL::GetColour() * m_fgCol;
  AmjuGL::PopMatrix();

  BuildTriList();
}

void GuiText2::BuildTriList()
{
  m_tris.clear(); // required for multi-line

  const Vec2f& pos = GetCombinedPos();

  const float oldSize = m_font->GetSize();
  m_font->SetSize(m_textSize * oldSize);
  std::string str = m_text; // split for multi-line

  if (m_isMulti)
  {
    Vec2f p(pos);
    for (const auto& s : m_lines)
    {
      BuildTriListForLine(s, p);
      p.y -= m_textSize * CHAR_HEIGHT_FOR_SIZE_1; // +ve Y is UP
    }
  }
  else
  {
    BuildTriListForLine(m_text, pos);
  }

  m_font->SetSize(oldSize);
}

void GuiText2::BuildTriListForLine(const std::string& str, const Vec2f& pos)
{
  const Vec2f& size = GetSize();

  float x = 0; // left edge of text, calculated below depending on justification
  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    x = pos.x;
    break;

  case AMJU_JUST_RIGHT:
    x = pos.x + size.x - GetTextWidth(str);
    break;

  case AMJU_JUST_CENTRE:
    const float w = GetTextWidth(str);
    x = pos.x + 0.5f * (size.x - w);
    break;
  }

  auto tris = m_font->MakeTriList(x, pos.y, str.c_str(), 1.f);
  for (auto& t : tris)
  {
    t.SetColour(m_combinedColour);
  }
  m_tris.insert(m_tris.end(), tris.begin(), tris.end());
}

bool GuiText2::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  return LoadTextInfo(f);
}

bool GuiText2::SaveTextInfo(File* f)
{
  // TODO Factor out common code
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
  if (m_isMulti)
  {
    s += ", multi";
  }
  // TODO bgCol, fgCol, scale, justification
  s += ", fgcol=" + ToHexString(m_fgCol);
  s += ", bgcol=" + ToHexString(m_bgCol);

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

bool GuiText2::LoadTextInfo(File* f)
{
  std::string text;
  if (!f->GetLocalisedString(&text, &m_preLocalisedText))
  {
    f->ReportError("GUI Text 2: Expected localised string");
    return false;
  }
 
  if (!ParseFontInfoLine(f))
  {
    return false;
  }

  // Don't do before setting font
  SetText(text); // does substitutions; not simple assignment

  return true;  
}

bool GuiText2::HandleAttrib(const std::string& s) 
{
  if (GuiTextBase::HandleAttrib(s)) return true;
  return false;
}

bool GuiText2::HandleAttrib(const std::string& key, const std::string& value)
{
  if (GuiTextBase::HandleAttrib(key, value)) return true;
  return false;
}

void GuiText2::SetText(const std::string& text)
{
  auto t = ReplaceEscaped(text);
  if (t == m_text) return;
  m_text = t;

  if (m_isMulti)
  {
    const float availWidth = GetSize().x;
    m_lines = WordWrap(m_text, availWidth, WidthFinder(this));
  }

  m_tris.clear(); // to rebuild
}

Texture* GuiText2::GetTexture() 
{
  return m_font->GetTexture();
}

void GuiText2::AddToTrilist(AmjuGL::Tris& tris) 
{
  tris.insert(tris.end(), m_tris.begin(), m_tris.end());
}

float GuiText2::GetTextWidth(const std::string& text) const
{
  const float textWidth = const_cast<GuiText2*>(this)->GetFont()->GetTextWidth(text);
  return textWidth;
}
}
