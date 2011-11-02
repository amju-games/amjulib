#include "OBB2.h"
#include <Vec3.h>
#include <Plane.h>
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

void OBB2::SetRotation(float rads)
{
  float c = cos(rads);
  float s = sin(rads);

  m_axes[0] = Vec2f( c, s);
  m_axes[1] = Vec2f(-s, c);
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


bool OBB2::Intersects(const Vec2f& p, Vec2f* contactNormal, float* penetrationDepth) const
{
  // Could make an OBB3 and write in terms of OBB3 intersects. But then we might get contact normal
  //  along z axis.
  // Uses same idea as OBB3 version

  Vec2f c[4];
  GetCorners(c);

  Vec3f c3[8] = 
  {
    Vec3f(c[0].x, c[0].y,  1), 
    Vec3f(c[0].x, c[0].y, -1), 
    Vec3f(c[1].x, c[1].y,  1), 
    Vec3f(c[1].x, c[1].y, -1), 
    Vec3f(c[2].x, c[2].y,  1), 
    Vec3f(c[2].x, c[2].y, -1), 
    Vec3f(c[3].x, c[3].y,  1), 
    Vec3f(c[3].x, c[3].y, -1) 
  };

  // Make 4 planes. Check if point is behind each one.
  Plane planes[4] =
  {
    Plane(c3[0], c3[2], c3[1]),
    Plane(c3[2], c3[4], c3[3]),
    Plane(c3[4], c3[6], c3[5]),
    Plane(c3[6], c3[0], c3[7]),
/*
    Plane(c3[0], c3[1], c3[2]),
    Plane(c3[2], c3[3], c3[4]),
    Plane(c3[4], c3[5], c3[6]),
    Plane(c3[6], c3[7], c3[0])
*/
  };

  Vec3f p3(p.x, p.y, 0);
  float bestPd = -99999.9f;
  Vec3f cn;
  for (int i = 0; i < 4; i++)
  {
    float pd = planes[i].Dist(p3);
    if (pd > 0)
    {
      return false; // in front of this plane, so not intersecting
    }
    if (pd > bestPd)
    {
      // Choose plane with min pen depth
      bestPd = pd;
      cn = planes[i].Normal();
    }
  }
  *contactNormal = Vec2f(cn.x, cn.y);
  Assert(cn.z == 0);
  *penetrationDepth = -bestPd;
  return true;
}

}
