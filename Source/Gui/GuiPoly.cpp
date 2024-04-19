#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <LoadVec2.h>
#include <Texture.h>
#include <Vec3.h>
#include "GuiPoly.h"
#include "GuiPolyEdit.h"
#include <AmjuFinal.h>

namespace Amju
{
GuiEdit* IGuiPoly::CreateEditor()
{
  return new GuiPolyEdit;
}

void IGuiPoly::CreateEditorDefault()
{
  AddControlPoint(Vec2f{ -.5, 0 });
  AddControlPoint(Vec2f{ .5, .1f });
  OnControlPointsChanged();
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

  Vec2f combinedPos = GetCombinedPos();
  Vec2f combinedSize = GetCombinedSize();
  Colour combinedColour = GetCombinedColour();
  if (m_previousCombinedPos != combinedPos ||
    m_previousCombinedSize != combinedSize ||
    m_previousCombinedColour != combinedColour)
  {
    m_previousCombinedPos = combinedPos;
    m_previousCombinedSize = combinedSize;
    m_previousCombinedColour = combinedColour;

    BuildTriList();
  }

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
  const Rect r = CalcRect();

  SetSize(r.GetSize());

  BuildTriList();
}

Rect IGuiPoly::CalcRect() const 
{
  Assert(!m_controlPoints.empty());
  auto compareXs = [](const Vec2f& a, const Vec2f& b) { return a.x < b.x; };
  auto compareYs = [](const Vec2f& a, const Vec2f& b) { return a.y < b.y; };
  float minx = std::min_element(m_controlPoints.begin(), m_controlPoints.end(), compareXs)->x;
  float maxx = std::max_element(m_controlPoints.begin(), m_controlPoints.end(), compareXs)->x;
  float miny = std::min_element(m_controlPoints.begin(), m_controlPoints.end(), compareYs)->y;
  float maxy = std::max_element(m_controlPoints.begin(), m_controlPoints.end(), compareYs)->y;

  const Vec2f pos = GetCombinedPos();
  Rect r(minx + pos.x, maxx + pos.x, miny + pos.y, maxy + pos.y);
  return r;
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

  if (!LoadPos(f))
  {
    f->ReportError("Poly: expected pos");
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
  if (!SaveTypeAndName(f))
  {
    return false;
  }
  
  if (!f->Write(ToString(GetLocalPos())))
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
  m_controlPoints.clear();
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
  AmjuGL::Tris tris;

  if (IsFilled())
  {
    tris = BuildFilledTriList();
  }
  if (IsOutline())
  {
    AmjuGL::Tris moreTris = BuildOutlineTriList();
    tris.insert(tris.end(), moreTris.begin(), moreTris.end());
  }
  m_triList = Amju::MakeTriList(tris);
}

const char* GuiPoly::NAME = "poly";

AmjuGL::Tris GuiPoly::BuildOutlineTriList()
{
  AmjuGL::Tris tris;

  int n = m_controlPoints.size();
  if (n < 2)
  {
    return tris;
  }

  const Vec2f pos = GetCombinedPos();
  const Colour colour = m_outlineColour * GetCombinedColour();

  constexpr float Z = .5f;
  constexpr float U = .5f;
  constexpr float V = .5f;
  const float w = m_cornerRadius;

  AmjuGL::Tri t;

  // Points of rectangle for segment, declared here so we shift the points, joining
  //  all the rectangles.
  Vec2f p[4];
  for (int i = 1; i < n; i++)
  {
    // Get direction for this segment, and perpendicular direction, so we can make an 
    //  oriented rectangle (actually trapezium, as width can vary).
    const Vec2f& p0 = pos + m_controlPoints[i - 1];
    const Vec2f& p1 = pos + m_controlPoints[i];
    const Vec2f dir = p1 - p0;
    Vec3f dir3(dir.x, dir.y, 0);
    dir3.Normalise();
    Vec3f perp3 = CrossProduct(dir3, Vec3f(0, 0, 1));
    perp3.Normalise();
    const Vec2f perp(perp3.x, perp3.y);

    if (i == 1)
    {
      // First segment, calc all 4 points
      p[0] = p0 + perp * w;
      p[1] = p1 + perp * w;
      p[2] = p1 - perp * w;
      p[3] = p0 - perp * w;
      // Right U: up to half way across texture
    }
    else
    {
      // Next segment: shift previous points and calc 2 new ones
      p[0] = p[1];
      p[3] = p[2];
      p[1] = p1 + perp * w;
      p[2] = p1 - perp * w;
    }

    AmjuGL::Vert verts[4] =
    {
      AmjuGL::Vert(p[0].x, p[0].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[1].x, p[1].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[2].x, p[2].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[3].x, p[3].y, Z, U, V, 0, 1.0f, 0)
    };

    t.Set(verts[0], verts[1], verts[2]);
    t.SetColour(colour);
    tris.push_back(t);
    t.Set(verts[0], verts[2], verts[3]);
    t.SetColour(colour);
    tris.push_back(t);
  }
  if (IsLoop())
  {
    const Vec2f& p0 = pos + m_controlPoints[n - 1];
    const Vec2f& p1 = pos + m_controlPoints[0];
    const Vec2f dir = p1 - p0;
    Vec3f dir3(dir.x, dir.y, 0);
    dir3.Normalise();
    Vec3f perp3 = CrossProduct(dir3, Vec3f(0, 0, 1));
    perp3.Normalise();
    const Vec2f perp(perp3.x, perp3.y);
    p[0] = p[1];
    p[3] = p[2];
    p[1] = p1 + perp * w;
    p[2] = p1 - perp * w;
    AmjuGL::Vert verts[4] =
    {
      AmjuGL::Vert(p[0].x, p[0].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[1].x, p[1].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[2].x, p[2].y, Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(p[3].x, p[3].y, Z, U, V, 0, 1.0f, 0)
    };

    t.Set(verts[0], verts[1], verts[2]);
    t.SetColour(m_outlineColour);
    tris.push_back(t);
    t.Set(verts[0], verts[2], verts[3]);
    t.SetColour(m_outlineColour);
    tris.push_back(t);
  }

  return tris;
}

AmjuGL::Tris GuiPoly::BuildFilledTriList()
{
  AmjuGL::Tris tris;
  AmjuGL::Tri t;

  constexpr float Z = 0.5f;
  constexpr float U = 0.5f;
  constexpr float V = 0.5f;

  const Vec2f pos = GetCombinedPos();
  const Colour colour = m_filledColour * GetCombinedColour();

  const int n = m_controlPoints.size() - 1;
  Assert(n > 0);
  for (int i = 1; i < n; i++)
  {
    AmjuGL::Vert verts[3] =
    {
      AmjuGL::Vert(pos.x + m_controlPoints[0].x,     pos.y + m_controlPoints[0].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(pos.x + m_controlPoints[i].x,     pos.y + m_controlPoints[i].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(pos.x + m_controlPoints[i + 1].x, pos.y + m_controlPoints[i + 1].y, Z, U, V, 0, 1.0f, 0),
    };

    t.Set(verts[0], verts[1], verts[2]);
    t.SetColour(colour);
    tris.push_back(t);
  }
  return tris;
}
}