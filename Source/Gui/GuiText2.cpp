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
  m_combinedColour = AmjuGL::GetColour();
  AmjuGL::PopMatrix();

  BuildTriList();
}

void GuiText2::BuildTriList()
{
  //m_tris.clear(); // required for multi line I expect

  const Vec2f& pos = GetCombinedPos();

  float oldSize = m_font->GetSize();
  m_font->SetSize(m_textSize * oldSize);

  m_tris = m_font->MakeTriList(pos.x, pos.y, m_text.c_str(), 1.f);
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
  return true;
}

bool GuiText2::HandleAttrib(const std::string& key, const std::string& value)
{
  return true;
}

void GuiText2::SetText(const std::string& text)
{
  auto t = ReplaceEscaped(text);
  if (t == m_text) return;
  m_text = t;

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
}

