// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <GuiFactory.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include <Timer.h>
#include <Vec3.h>
#include "GuiSpline.h"

namespace Amju
{
const char* GuiSpline::NAME = "spline";

void GuiSpline::Reset()
{
  m_triList = nullptr;
  m_index = 0;
}

static Vec2f CatmullRomSpline(float t, Vec2f p1, Vec2f p2, Vec2f p3, Vec2f p4)
{
  const float t2 = t*t;
  const float t3 = t*t*t;
  Vec2f v; // Interpolated point

  /* Catmull Rom spline Calculation */
  v.x = ((-t3 + 2 * t2 - t)*(p1.x) + (3 * t3 - 5 * t2 + 2)*(p2.x) + (-3 * t3 + 4 * t2 + t)* (p3.x) + (t3 - t2)*(p4.x)) / 2;
  v.y = ((-t3 + 2 * t2 - t)*(p1.y) + (3 * t3 - 5 * t2 + 2)*(p2.y) + (-3 * t3 + 4 * t2 + t)* (p3.y) + (t3 - t2)*(p4.y)) / 2;

  return v;
}

void GuiSpline::BuildFilledTriList()
{

}

void GuiSpline::BuildOutlineTriList()
{
  AmjuGL::Tris tris;

  // Points of rectangle for segment, declared here so we shift the points, joining
  //  all the rectangles.
  Vec2f p[4];
  // UV coords, also shifted. 
  float u0 = 0.f;
  float u1 = 0.f;
  const float v0 = 0;
  const float v1 = 1;
  float totalLength = 0;

  for (int i = 1; i < m_index; i++)
  {
    // Get direction for this segment, and perpendicular direction, so we can make an 
    //  oriented rectangle (actually trapezium, as width can vary).
    const Vec2f& p0 = m_points[i - 1];
    const Vec2f& p1 = m_points[i];
    Vec2f dir = p1 - p0;
    float segLength = sqrtf(dir.SqLen());
    Vec3f dir3(dir.x, dir.y, 0);
    dir3.Normalise();
    Vec3f perp3 = CrossProduct(dir3, Vec3f(0, 0, 1));
    perp3.Normalise();
    Vec2f perp(perp3.x, perp3.y);

    // Calc line width here: either linear interp between start and end...
#ifdef LINE_WIDTH_LINEAR
    // d is proportion of length covered, 0..1
    float d = static_cast<float>(i) / static_cast<float>(m_points.size());
#endif

    // For music score curves, thickest in middle. So d varies from 0 at the ends to
    //  1 half way.
    float d = sin(static_cast<float>(i) / static_cast<float>(m_points.size()) * static_cast<float>(M_PI));
    d *= d;

    float w = m_startWidth + (m_endWidth - m_startWidth) * d; 

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
      // Shift, and calc new U coord below
      u0 = u1;
    }
    totalLength += segLength;
    // Calculate next u-coord
    // TODO Short strokes may break this?
    if (totalLength < m_totalLength * 0.5f)
    {
      u1 = std::min(0.5f, totalLength / w * 0.33f);
    }
    else
    {
      float a = m_totalLength - totalLength;
      if (a < (w * 1.5f))
      {
        u1 = (1.f - a / (w * 1.5f)) * 0.5f + 0.5f;
      }
    }

    AmjuGL::Tri t[2];

    const float Z = 0.5f;
    AmjuGL::Vert verts[4] =
    {
      AmjuGL::Vert(p[0].x, p[0].y, Z, u0, v0, 0, 1.0f, 0),
      AmjuGL::Vert(p[1].x, p[1].y, Z, u1, v0, 0, 1.0f, 0),
      AmjuGL::Vert(p[2].x, p[2].y, Z, u1, v1, 0, 1.0f, 0),
      AmjuGL::Vert(p[3].x, p[3].y, Z, u0, v1, 0, 1.0f, 0)
    };
        
    t[0].Set(verts[0], verts[1], verts[2]);
    t[1].Set(verts[0], verts[2], verts[3]);
    t[0].SetColour(m_outlineColour);
    t[1].SetColour(m_outlineColour);

    tris.push_back(t[0]);
    tris.push_back(t[1]);
  }
  m_triList = Amju::MakeTriList(tris);
}

void GuiSpline::SetWidths(float w1, float w2)
{
  m_startWidth = w1;
  m_endWidth = w2;
}

bool GuiSpline::Save(File* f)
{
  if (!GuiElement::SaveTypeAndName(f))
  {
    return false;
  }
  // NOT saving pos and size
  if (!f->WriteFloat(m_startWidth))
  {
    return false;
  }
  if (!f->WriteFloat(m_endWidth))
  {
    return false;
  }

  if (!f->Write(CreateAttribString()))
  {
    return false;
  }

  return SavePoints(f);

  return true;
}

bool GuiSpline::Load(File* f)
{
  m_isLoop = false; 

  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Spline: expected name");
    Assert(0);
    return false;
  }

  // NOT saving pos and size

  // Attribs
  std::string attribs;
  if (!f->GetDataLine(&attribs))
  {
    f->ReportError("Spline: Expected attribs string.");
    return false;
  }
  ParseAttribString(attribs);

  // We don't read the control points from a separate file any more.
  if (!LoadPoints(f))
  {
    return false;
  }

  OnControlPointsChanged();

  return true;
}

bool GuiSpline::ParseOneAttrib(const Strings& strs)
{
  if (strs.size() == 2)
  {
    if (strs[0] == "w0")
    {
      m_startWidth = ToFloat(strs[1]);
      return true;
    }
    else if (strs[0] == "w1")
    {
      m_endWidth = ToFloat(strs[1]);
      return true;
    }
  }
  return IGuiPoly::ParseOneAttrib(strs);
}

std::string GuiSpline::CreateAttribString() const
{
  std::string s = IGuiPoly::CreateAttribString();
  s += ", w0=" + ToString(m_startWidth);
  s += ", w1=" + ToString(m_endWidth);
  return s;
} 

bool GuiSpline::LoadPoints(File* f)
{
  if (!IGuiPoly::LoadPoints(f))
  {
    return false;
  }
  return true;
}

void GuiSpline::MakeInBetweenPoints()
{
  // Make in between points from control points
  m_points.clear();
  m_totalLength = 0;

  // Copy and add extras at front and back
  ControlPoints controlPoints(m_controlPoints);
  controlPoints.insert(controlPoints.begin(), controlPoints.front());
  if (IsLoop())
  {
    // TODO This needs work in BuildTris.
    controlPoints.push_back(controlPoints.front());
    controlPoints.push_back(controlPoints.front());
  }
  else
  {
    controlPoints.push_back(controlPoints.back());
  }

  const int n = controlPoints.size() - 3;
  for (int i = 0; i < n; i++)
  {
    float t = 0;
    while (t < 1.0f)
    {
      Vec2f v = CatmullRomSpline(t, controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3]);
//      v.x *= m_size.x;
//      v.y *= m_size.y;

      // Add to total length
      if (!m_points.empty())
      {
        m_totalLength += sqrtf((v - m_points.back()).SqLen());
      }

      m_points.push_back(v);

      t += 0.04f; // TODO 
    }
  }
}

void GuiSpline::OnControlPointsChanged()
{
  MakeInBetweenPoints();
  m_index = m_points.size(); // build the entire line
  IGuiPoly::OnControlPointsChanged();
}

void GuiSpline::Animate(float d)
{
  int oldIndex = m_index;
  m_index = static_cast<int>(static_cast<float>(m_points.size()) * d);
  if (m_index > static_cast<int>(m_points.size()))
  {
    m_index = static_cast<int>(m_points.size());
  }

  if (oldIndex != m_index)
  {
    BuildTriList();
  }
}

}

