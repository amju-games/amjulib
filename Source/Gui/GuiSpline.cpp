// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <DoOnce.h>
#include <GuiFactory.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include <Timer.h>
#include <Vec3.h>
#include "GuiSpline.h"

namespace
{
  using namespace Amju;

  // Safely calculate knot values
  static float GetKnotTime(float t_prev, Vec2f p_prev, Vec2f p_curr, float alpha)
  {
    float dx = p_curr.x - p_prev.x;
    float dy = p_curr.y - p_prev.y;
    float sq_dist = dx * dx + dy * dy;

    // If the points are identical, the interval is exactly 0
    if (sq_dist < 1e-7f) return t_prev;

    return t_prev + std::pow(sq_dist, alpha * 0.5f);
  }

  // Zero-safe linear interpolation helper for Barry-Goldman steps
  static Vec2f SafeLerp(Vec2f a, Vec2f b, float t_start, float t_end, float global_t)
  {
    float denominator = t_end - t_start;
    // If time span is zero, the control points are overlapping: return one of them safely.
    if (std::abs(denominator) < 1e-7f) return a;

    return a * ((t_end - global_t) / denominator) + b * ((global_t - t_start) / denominator);
  }

  static Vec2f CatmullRomSpline(float t, Vec2f p1, Vec2f p2, Vec2f p3, Vec2f p4, float alpha = 0.5f)
  {
    float t1 = 0.0f;
    float t2 = GetKnotTime(t1, p1, p2, alpha);
    float t3 = GetKnotTime(t2, p2, p3, alpha);
    float t4 = GetKnotTime(t3, p3, p4, alpha);

    // If the core segment between p2 and p3 has zero length, it's just a single static point
    if (std::abs(t3 - t2) < 1e-7f) return p2;

    // Map input t (0.0 to 1.0) into the global curve time
    float global_t = t2 + t * (t3 - t2);

    // 1st level blending (Safe from t1==t2, t2==t3, t3==t4)
    Vec2f a1 = SafeLerp(p1, p2, t1, t2, global_t);
    Vec2f a2 = SafeLerp(p2, p3, t2, t3, global_t);
    Vec2f a3 = SafeLerp(p3, p4, t3, t4, global_t);

    // 2nd level blending (Safe from wide duplicate spans like t1==t3)
    Vec2f b1 = SafeLerp(a1, a2, t1, t3, global_t);
    Vec2f b2 = SafeLerp(a2, a3, t2, t4, global_t);

    // Final blending (We already checked t3 - t2 != 0 above, so this is guaranteed safe)
    return SafeLerp(b1, b2, t2, t3, global_t);
  }
}

namespace Amju
{
const char* GuiSpline::NAME = "spline";

FunctionFactory<GuiSpline::WidthFunc> GuiSpline::s_widthFuncFactory;

GuiSpline::WidthFunc GuiSpline::WidthFuncFactoryCreate(std::string& str)
{
  auto wf = s_widthFuncFactory.Create(str);
  if (wf) return wf;
  str.clear();
  return nullptr;
}

bool GuiSpline::AddWidthFunc(const std::string& str, WidthFunc f)
{
  return s_widthFuncFactory.Add(str, f); 
}

GuiSpline::GuiSpline()
{
  do_once
  {
    AddWidthFunc("music", MusicCurveWidthFunc);
    AddWidthFunc("lerp", LerpWidthFunc);
    AddWidthFunc("log", LogWidthFunc);
    AddWidthFunc("exp", ExpWidthFunc);
  }
}

AmjuGL::Tris GuiSpline::BuildFilledTriList()
{
  AmjuGL::Tris tris;
  AmjuGL::Tri t;

  constexpr float Z = 0.5f;
  constexpr float U = 0.5f;
  constexpr float V = 0.5f;

  // We use the combined pos when we create the in-between points, it's not missing.
  const Colour colour = m_filledColour * GetCombinedColour();

  const int n = static_cast<int>(m_points.size()) - 1;
  Assert(n > 0);
  for (int i = 1; i < n; i++)
  {
    AmjuGL::Vert verts[3] =
    {
      AmjuGL::Vert(m_points[0].x,     m_points[0].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(m_points[i].x,     m_points[i].y,     Z, U, V, 0, 1.0f, 0),
      AmjuGL::Vert(m_points[i + 1].x, m_points[i + 1].y, Z, U, V, 0, 1.0f, 0),
    };

    t.Set(verts[0], verts[1], verts[2]);
    t.SetColour(colour);
    tris.push_back(t);
  }
  return tris;
}

AmjuGL::Tris GuiSpline::BuildOutlineTriList()
{
  AmjuGL::Tris tris;

  // Don't call this if no points!
//  Assert(m_totalLength > 0);

  // Points of rectangle for segment, declared here so we shift the points, joining
  //  all the rectangles.
  Vec2f p[4];
  // UV coords, also shifted. 
  float u0 = 0.f;
  float u1 = 0.f;
  const float v0 = 0;
  const float v1 = 1;
  float accLength = 0;

  const Colour colour = m_outlineColour * GetCombinedColour();

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

    accLength += segLength;

    // Calc width of line at this point along it
    float w = 1.f;
    if (m_widthFunc)
    {
      const float t = accLength / m_totalLength;
      w = m_widthFunc(t, m_startWidth, m_endWidth);
    }
    else
    {
      const float t = accLength / m_totalLength;
      w = DefaultWidthFunction(t, m_startWidth, m_endWidth);
    }

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
    // Calculate next u-coord
    // TODO Short strokes may break this?
    if (accLength < m_totalLength * 0.5f)
    {
      u1 = std::min(0.5f, accLength / w * 0.33f);
    }
    else
    {
      float a = m_totalLength - accLength;
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
    t[0].SetColour(colour);
    t[1].SetColour(colour);

    tris.push_back(t[0]);
    tris.push_back(t[1]);
  }
  return tris;
}

void GuiSpline::SetWidths(float w1, float w2)
{
  m_startWidth = w1;
  m_endWidth = w2;
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
    else if (strs[0] == "width_func")
    {
      m_widthFuncName = strs[1];
      m_widthFunc = WidthFuncFactoryCreate(m_widthFuncName);
      return true;
    }
    else if (strs[0] == "alpha")
    {
      m_alpha = ToFloat(strs[1]);
      return true;
    }
    else if (strs[0] == "num_points")
    {
      m_numPoints = ToInt(strs[1]);
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
  s += ", alpha=" + ToString(m_alpha);
  s += ", num_points=" + ToString(m_numPoints);
  if (!m_widthFuncName.empty())
  {
    s += ", width_func=" + m_widthFuncName;
  }
  return s;
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

  const Vec2f pos = GetCombinedPos();

  float tInc = 5;
  if (m_numPoints > 0)
  {
    tInc = 1.f / static_cast<float>(m_numPoints);
  }

  const int n = static_cast<int>(controlPoints.size()) - 3;
  for (int i = 0; i < n; i++)
  {
    float t = 0;
    while (t < 1.0f)
    {
      Vec2f v = CatmullRomSpline(t, 
        controlPoints[i], 
        controlPoints[i + 1], 
        controlPoints[i + 2], 
        controlPoints[i + 3], 
        m_alpha);

      // We should probably get combined size here, IF we want control points
      //  to be scalable.
//      v.x *= m_size.x;
//      v.y *= m_size.y;

      // Add to total length
      if (!m_points.empty())
      {
        m_totalLength += sqrtf((v - m_points.back()).SqLen());
      }

      m_points.push_back(v + pos);

      t += tInc; 
    }
  }
}

void GuiSpline::OnControlPointsChanged()
{
  MakeInBetweenPoints();
  m_index = static_cast<int>(m_points.size()); // build the entire line
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

