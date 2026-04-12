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
  const Vec2f& size = GetSize();
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
  //m_tris.clear(); // required for multi line I expect

  const Vec2f& pos = GetCombinedPos();

  const float oldSize = m_font->GetSize();
  m_font->SetSize(m_textSize * oldSize);
  std::string str = m_text; // split for multi-line

  Vec2f size = GetSize();
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
    // divide by font size required to fix centering, but TODO: why??
    const float w = GetTextWidth(str) / m_textSize;
    x = pos.x + 0.5f * (size.x - w);
    break;
  }

  m_tris = m_font->MakeTriList(x, pos.y, m_text.c_str(), 1.f);
  for (auto& t : m_tris)
  {
    t.SetColour(m_combinedColour);
  }

  m_font->SetSize(oldSize);
}

bool GuiText2::Load(File* f) 
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

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

  if (key == "fgcol")
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
  return false;
}

struct WidthFinder2
{
  WidthFinder2(GuiText2* g) : m_guiText(g) {}
  float operator()(const std::string& s)
  {
    return m_guiText->GetTextWidth(s);
  }
  GuiText2* m_guiText;
};

void GuiText2::SetText(const std::string& text)
{
  auto t = ReplaceEscaped(text);
  if (t == m_text) return;
  m_text = t;

  if (m_isMulti)
  {
    const float availWidth = GetSize().x;
    m_lines = WordWrap(m_text, availWidth, WidthFinder2(this));
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
  return textWidth * m_textSize;
}
}
