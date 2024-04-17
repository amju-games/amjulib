#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Texture.h>
#include "GuiPoly.h"
#include "GuiPolyEdit.h"
#include <AmjuFinal.h>

namespace Amju
{
GuiEdit* IGuiPoly::CreateEditor()
{
  return new GuiPolyEdit;
}

bool IGuiPoly::IsLoop() const 
{ 
  return m_isLoop; 
}

void IGuiPoly::SetIsLoop(bool isLoop) 
{
  if (m_isLoop == isLoop)
  {
    return;
  }
  m_isLoop = isLoop; 
  BuildTriList();
}

const Colour& IGuiPoly::GetFilledColour() const
{
  return m_filledColour;
}

void IGuiPoly::SetFilledColour(const Colour& colour)
{
  if (m_filledColour == colour)
  {
    return;
  }
  m_filledColour = colour;
  BuildFilledTriList();
}

const Colour& IGuiPoly::GetOutlineColour() const
{
  return m_outlineColour;
}

void IGuiPoly::SetOutlineColour(const Colour& colour)
{
  if (m_outlineColour == colour)
  {
    return;
  }
  m_outlineColour = colour;
  BuildOutlineTriList();
}

void IGuiPoly::Draw() 
{
  if (!IsVisible())
  {
    return;
  }

  // TODO use circle texture, draw rounded corners.
  // We need a white texture, disabling textures doesn't work right now, needs a different shader.
  // TODO Set once before drawing all batched Polys
  static Texture* texture = (Texture*)TheResourceManager::Instance()->GetRes("Image/circle.png");
  Assert(texture);
  texture->SetWrapMode(AmjuGL::AMJU_TEXTURE_CLAMP); 
  texture->UseThisTexture();

  AmjuGL::Draw(m_triList);
}

void IGuiPoly::AddControlPoint(const Vec2f& p)
{
  m_controlPoints.push_back(p);
}

const IGuiPoly::ControlPoints& IGuiPoly::GetControlPoints() const
{
  return m_controlPoints;
}

IGuiPoly::ControlPoints& IGuiPoly::GetControlPoints()
{
  return m_controlPoints;
}

void IGuiPoly::OnControlPointsChanged()
{
  // Set pos and size from control points
  Assert(!m_controlPoints.empty());
  auto compareXs = [](const Vec2f& a, const Vec2f& b) { return a.x < b.x; };
  auto compareYs = [](const Vec2f& a, const Vec2f& b) { return a.y < b.y; };
  float minx = std::min_element(m_controlPoints.begin(), m_controlPoints.end(), compareXs)->x;
  float maxx = std::max_element(m_controlPoints.begin(), m_controlPoints.end(), compareXs)->x;
  float miny = std::min_element(m_controlPoints.begin(), m_controlPoints.end(), compareYs)->y;
  float maxy = std::max_element(m_controlPoints.begin(), m_controlPoints.end(), compareYs)->y;

  SetSize(Vec2f(maxx - minx, maxy - miny));
  SetLocalPos(Vec2f(minx, maxy));

  BuildTriList();
}

bool IGuiPoly::ParseAttribString(const std::string& attribStr)
{
  const Strings strs = Split(attribStr, ',');
  Assert(!strs.empty());

  for (const std::string& s : strs)
  {
    const Strings s2 = Split(s, '=');
    if (!ParseOneAttrib(s2))
    {
      return false;
    }
  }
  return true;
}

bool IGuiPoly::ParseOneAttrib(const Strings& s2)
{
  if (s2.size() == 1)
  {
    if (s2[0] == "loop")
    {
      m_isLoop = true;
    }
  }
  else if (s2[0] == "style")
  {
    m_style = static_cast<Style>(ToInt(s2[1]));
  }
  else if (s2[0] == "outCol")
  {
    m_outlineColour = FromHexString(s2[1]);
  }
  else if (s2[0] == "fillCol")
  {
    m_filledColour = FromHexString(s2[1]);
  }
  else if (s2[0] == "radius")
  {
    m_cornerRadius = ToFloat(s2[1]);
  }
  return true;
}

std::string IGuiPoly::CreateAttribString() const
{
  std::string s = "style=" + ToString(static_cast<int>(m_style));
  if (m_isLoop)
  {
    s += ", loop";
  }
  s += ", outCol=" + ToHexString(m_outlineColour);
  s += ", fillCol=" + ToHexString(m_filledColour);
  s += ", radius=" + ToString(m_cornerRadius);

  return s;
}

bool IGuiPoly::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Poly: expected name");
    Assert(0);
    return false;
  }

  std::string attribs;
  if (!f->GetDataLine(&attribs))
  {
    f->ReportError("Poly: expected attribs string");
    Assert(0);
    return false;
  }
  if (!ParseAttribString(attribs))
  {
    f->ReportError("Poly: failed to parse attribs string");
    Assert(0);
    return false;
  }

  if (!LoadPoints(f))
  {
    return false;
  }
  OnControlPointsChanged();
  return true;
}

bool IGuiPoly::Save(File* f)
{
  if (SaveTypeAndName(f))
  {
    return false;
  }
  
  if (!f->Write(CreateAttribString()))
  {
    return false;
  }

  return SavePoints(f);
}

bool IGuiPoly::LoadPoints(File* f)
{
  // Load control points
  std::string line;
  while (f->GetDataLine(&line))
  {
    // Split line
    Strings strs = Split(line, ',');
    const int n = strs.size();
    if (n != 2)
    {
      // So a string such as 'end' signals the end of the points list.
      break;
    }
    const Vec2f pos(ToFloat(strs[0]), ToFloat(strs[1]));
    m_controlPoints.push_back(pos);
  }

  return true;
}

bool IGuiPoly::SavePoints(File* f)
{
  for (const Vec2f& p : m_controlPoints)
  {
    if (!f->Write(ToString(p.x) + ", " + ToString(p.y)))
    {
      return false;
    }
  }
  if (!f->Write("end"))
  {
    return false;
  }
  return true;
}

bool IGuiPoly::IsFilled() const
{
  return m_style == Style::FILLED || m_style == Style::FILLED_AND_OUTLINE;
}

bool IGuiPoly::IsOutline() const
{
  return m_style == Style::OUTLINE || m_style == Style::FILLED_AND_OUTLINE;
}

IGuiPoly::Style IGuiPoly::GetStyle() const
{
  return m_style;
}

void IGuiPoly::SetStyle(Style s)
{
  if (m_style == s)
  {
    return;
  }
  m_style = s;
  BuildTriList();
}

void IGuiPoly::BuildTriList()
{
  if (IsFilled())
  {
    BuildFilledTriList();
  }
  if (IsOutline())
  {
    BuildOutlineTriList();
  }
}

const char* GuiPoly::NAME = "poly";

void GuiPoly::BuildOutlineTriList()
{

}

void GuiPoly::BuildFilledTriList()
{
  AmjuGL::Tris tris;
  AmjuGL::Tri t;

  constexpr float Z = 0.5f;
  constexpr float U = 0.5f;
  constexpr float V = 0.5f;

  const int n = m_controlPoints.size() - 1;
  Assert(n > 0);
  for (int i = 1; i < n; i++)
  {
    AmjuGL::Vert verts[3] =
    {
      AmjuGL::Vert(m_controlPoints[0].x,     m_controlPoints[0].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(m_controlPoints[i].x,     m_controlPoints[i].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(m_controlPoints[i + 1].x, m_controlPoints[i + 1].y, Z, U, V, 0, 1.0f, 0),
    };

    t.Set(verts[0], verts[1], verts[2]);
    t.SetColour(m_filledColour);
    tris.push_back(t);
  }
  m_triList = Amju::MakeTriList(tris);
}
}