#include "OBB2.h"
#include <AmjuAssert.h>

namespace Amju
{
OBB2::OBB2()
{
}

OBB2::OBB2(const Vec2f& centre, const Vec2f& xAxis, const Vec2f& extents)
  : m_centre(centre), m_extents(extents) 
{
  SetXAxis(xAxis); 
}

void OBB2::SetCentre(const Vec2f& centre)
{
  m_centre = centre;
}

void OBB2::SetXAxis(const Vec2f& axis)
{
  Assert(axis.SqLen() > 0);

  m_axes[0] = axis;
  m_axes[0].Normalise();
  m_axes[1] = Vec2f(-m_axes[0].y, m_axes[0].x); 
}

void OBB2::SetExtents(const Vec2f& extents)
{
  m_extents = extents;
}

const Vec2f& OBB2::GetCentre() const
{
  return m_centre;
}

const Vec2f& OBB2::GetAxis(int n) const
{
  return m_axes[n];
}

const Vec2f& OBB2::GetExtents() const
{
  return m_extents;
}

void OBB2::GetCorners(Vec2f corners[4]) const
{
  // Axes scaled by extents
  Vec2f e1 = GetAxis(0) * GetExtents().x;
  Vec2f e2 = GetAxis(1) * GetExtents().y;

  // Corners
  corners[0] = Vec2f(GetCentre() + e1 + e2);
  corners[1] = Vec2f(GetCentre() + e1 - e2);
  corners[2] = Vec2f(GetCentre() - e1 - e2);
  corners[3] = Vec2f(GetCentre() - e1 + e2);
}

bool Intersects(const OBB2& b1, const OBB2& b2)
{
  // 4 axes - b1.axes[0], b1.axes[1], b2.axes[0], b2.axes[1]
  Vec2f axis[4] = { b1.GetAxis(0), b1.GetAxis(1), b2.GetAxis(0), b2.GetAxis(1) };

  // For each axis, project box corners onto axis. Keep track of min and max for box.
  // If projections don't overlap, this is a separating axis.
  Vec2f c[2][4];
  b1.GetCorners(&c[0][0]);
  b2.GetCorners(&c[1][0]);
  float minmax[2][2] = { { 999999.9f, -999999.9f }, { 999999.9f, -999999.9f } };

  for (int i = 0; i < 4; i++) // 4 axes
  {
    for (int k = 0; k < 4; k++)  // 4 corners per box
    {
      float projected[2] = { 
        DotProduct(c[0][k], axis[i]), 
        DotProduct(c[1][k], axis[i]) };

      for (int j = 0; j < 2; j++)
      {
        if (projected[j] < minmax[j][0])
        {
          minmax[j][0] = projected[j];
        }
        if (projected[j] > minmax[j][1])
        {
          minmax[j][1] = projected[j];
        }
      }
    }

    // Overlap?
    if (minmax[0][0] > minmax[1][1] ||  minmax[1][0] > minmax[0][1])
    {
      // No overlap. This is a separating axis
      return false;
    }
  }

  return true;
}
}
