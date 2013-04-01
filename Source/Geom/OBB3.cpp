#include <AmjuFirst.h>
#include <iostream>
#include "OBB3.h"
#include "ClipLineSegBox.h"
#include <AmjuAssert.h>
#include <AmjuFinal.h>

namespace Amju
{
OBB3::OBB3()
{
  m_axes[0] = Vec3f(1, 0, 0);
  m_axes[1] = Vec3f(0, 1, 0);
  m_axes[2] = Vec3f(0, 0, 1);
}

OBB3::OBB3(const Vec3f& centre, const Vec3f& xAxis, const Vec3f& yAxis, const Vec3f& extents)
{
  m_centre = centre;
  m_extents = extents;
  SetAxes(xAxis, yAxis);
}

OBB3::OBB3(const Vec3f& centre, const Quaternion& orientation, const Vec3f& extents)
{
  m_centre = centre;
  m_extents = extents;
  SetOrientation(orientation);
}

void OBB3::SetOrientation(const Quaternion& q)
{
  m_axes[0] = q.RotateVec(Vec3f(1, 0, 0));
  m_axes[1] = q.RotateVec(Vec3f(0, 1, 0));
  m_axes[2] = q.RotateVec(Vec3f(0, 0, 1));
}

void OBB3::SetCentre(const Vec3f& centre)
{
  m_centre = centre;
}

void OBB3::SetAxes(const Vec3f& xAxis, const Vec3f& yAxis)
{
  m_axes[0] = xAxis;
  m_axes[1] = yAxis;
  m_axes[0].Normalise();
  m_axes[1].Normalise();
  m_axes[2] = CrossProduct(m_axes[0], m_axes[1]);
}

void OBB3::SetExtents(const Vec3f& extents)
{
  m_extents = extents;
}

const Vec3f& OBB3::GetCentre() const
{
  return m_centre;
}

const Vec3f& OBB3::GetAxis(int n) const
{
  return m_axes[n];
}

const Vec3f& OBB3::GetExtents() const
{
  return m_extents;
}

bool OBB3::Intersects(const LineSeg& e, LineSeg* clip, Vec3f* contactNormal, float* penDepth) const
{
  LineSeg c;
  // Call lineseg-box clip function
  if (!Clip(e, *this, &c))
  {
    return false;
  }
  *clip = c;
  Vec3f mid = (c.p0 + c.p1) * 0.5f;
  // Find nearest face to mid, giving contact normal and pen depth
/*
std::cout << "Edge-box test, finding contact normal and pen depth but it asserts...\n";
std::cout << " This box size: x: " << m_extents.x << " y: " << m_extents.y << " z: " << m_extents.z << "\n";
std::cout << " Orig line seg: " 
  << " p0.x: " << e.p0.x << " y: " << e.p0.y << " z: " << e.p0.z 
  << " p1.x: " << e.p1.x << " y: " << e.p1.y << " z: " << e.p1.z << "\n"; 

std::cout << " Clipped line seg in box: " 
  << " p0.x: " << c.p0.x << " y: " << c.p0.y << " z: " << c.p0.z 
  << " p1.x: " << c.p1.x << " y: " << c.p1.y << " z: " << c.p1.z << "\n"; 
std::cout << " expecting this mid point to be in box: x " << mid.x << " y: " << mid.y << " z: " << mid.z << "\n";
*/

  bool b = Intersects(mid, contactNormal, penDepth);
  //Assert(b); // clipped line seg is supposed to be within this obb
  if (!b)
  {
std::cout << "Unexpected: mid point of clipped line not in box!\n";
    return false;
  }
  return true;
}

bool OBB3::Intersects(const Vec3f& p, Vec3f* contactNormal, float* penetrationDepth) const
{
  // Make 6 planes. Check if point is behind each one. 
  Plane planes[6];
  GetPlanes(planes);
  
  float bestPd = -999999.9f;
  Vec3f cn;
  for (int i = 0; i < 6; i++)
  {
    float pd = planes[i].Dist(p);
//std::cout << " point-box intersection test: pd for plane " << i << " is " << pd << "\n";

    if (pd >= 0)
    {
      return false; // in front of this plane, so not intersecting   
    }
    if (pd > bestPd)
    {
      // Choose plane with min pen depth
      bestPd = pd;
      //cn = planes[i].Normal();
    }
    // All normals contribute to overall contact normal, but if a point is very close to a plane
    //  it contributes more. This is an idea to try to get a more realistic overall contact normal.
    float nm = 1.0f / -pd; // or pd squared perhaps, etc., so the further we are from a plane, the
    // less it contributes. -pd because pd is negative
    cn += planes[i].Normal() * nm;
  }

  // Make sure penetration depth is correct now that normal is not perpendicular to any face
  
  *penetrationDepth = -bestPd;
  cn.Normalise();  

  *contactNormal = cn;
  return true;
}

void OBB3::GetEdges(LineSeg edges[12]) const
{
  Vec3f c[8];
  GetCorners(c);

  edges[0]  = LineSeg(c[0], c[1]);
  edges[1]  = LineSeg(c[1], c[2]);
  edges[2]  = LineSeg(c[2], c[3]);
  edges[3]  = LineSeg(c[3], c[0]);

  edges[4]  = LineSeg(c[4], c[5]);
  edges[5]  = LineSeg(c[5], c[6]);
  edges[6]  = LineSeg(c[6], c[7]);
  edges[7]  = LineSeg(c[7], c[4]);

  edges[8]  = LineSeg(c[0], c[4]);
  edges[9]  = LineSeg(c[1], c[5]);
  edges[10] = LineSeg(c[2], c[6]);
  edges[11] = LineSeg(c[3], c[7]);
}

void OBB3::GetPlanes(Plane planes[6]) const
{
  Vec3f c[8];
  GetCorners(c);

  planes[0] = Plane(c[4], c[1], c[0]);
  planes[1] = Plane(c[3], c[6], c[7]);
  planes[2] = Plane(c[3], c[0], c[2]);
  planes[3] = Plane(c[4], c[7], c[5]);
  planes[4] = Plane(c[3], c[4], c[0]);
  planes[5] = Plane(c[5], c[2], c[1]);
}

void OBB3::GetCorners(Vec3f corners[8]) const
{
  // Axes scaled by extents
  Vec3f e1 = GetAxis(0) * GetExtents().x;
  Vec3f e2 = GetAxis(1) * GetExtents().y;
  Vec3f e3 = GetAxis(2) * GetExtents().z;

  // Corners
  corners[0] = Vec3f(GetCentre() + e1 + e2 - e3);
  corners[1] = Vec3f(GetCentre() + e1 - e2 - e3);
  corners[2] = Vec3f(GetCentre() - e1 - e2 - e3);
  corners[3] = Vec3f(GetCentre() - e1 + e2 - e3);

  corners[4] = Vec3f(GetCentre() + e1 + e2 + e3);
  corners[5] = Vec3f(GetCentre() + e1 - e2 + e3);
  corners[6] = Vec3f(GetCentre() - e1 - e2 + e3);
  corners[7] = Vec3f(GetCentre() - e1 + e2 + e3);
}

bool Intersects(const OBB3& b1, const OBB3& b2)
{
  // TODO Not optimised
  // 15 axes 
  Vec3f axis[15] = 
  { 
    b1.GetAxis(0), 
    b1.GetAxis(1), 
    b1.GetAxis(2), 

    b2.GetAxis(0), 
    b2.GetAxis(1),
    b2.GetAxis(2),

    CrossProduct(b1.GetAxis(0), b2.GetAxis(0)),
    CrossProduct(b1.GetAxis(0), b2.GetAxis(1)),
    CrossProduct(b1.GetAxis(0), b2.GetAxis(2)),

    CrossProduct(b1.GetAxis(1), b2.GetAxis(0)),
    CrossProduct(b1.GetAxis(1), b2.GetAxis(1)),
    CrossProduct(b1.GetAxis(1), b2.GetAxis(2)),

    CrossProduct(b1.GetAxis(2), b2.GetAxis(0)),
    CrossProduct(b1.GetAxis(2), b2.GetAxis(1)),
    CrossProduct(b1.GetAxis(2), b2.GetAxis(2)),
  };

  // For each axis, project box corners onto axis. Keep track of min and max for box.
  // If projections don't overlap, this is a separating axis.
  Vec3f c[2][8]; // 2 boxes, 8 corners each
  b1.GetCorners(&c[0][0]);
  b2.GetCorners(&c[1][0]);

  for (int i = 0; i < 15; i++) // for each axis
  {
    // Min and max extents for each box for the current axis
    float minmax[2][2] = { { 999999.9f, -999999.9f }, { 999999.9f, -999999.9f } };

    for (int k = 0; k < 8; k++)  // 8 corners per box
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
