#ifndef OBB3_H
#define OBB3_H

#include <Vec3.h>
#include <Quaternion.h>
#include <Plane.h>
#include <Line3.h>

namespace Amju
{
// 3D oriented bounding box
class OBB3
{
public:
  OBB3();
  OBB3(const Vec3f& centre, const Vec3f& xAxis, const Vec3f& yAxis, const Vec3f& extents);
  OBB3(const Vec3f& centre, const Quaternion& orientation, const Vec3f& extents);

  void SetCentre(const Vec3f& centre);
  void SetAxes(const Vec3f& xAxis, const Vec3f& yAxis);
  void SetExtents(const Vec3f& extents);
  void SetOrientation(const Quaternion& q);

  const Vec3f& GetCentre() const;
  const Vec3f& GetAxis(int n) const;
  const Vec3f& GetExtents() const;

  void GetCorners(Vec3f[8]) const;
  void GetPlanes(Plane planes[6]) const;
  void GetEdges(LineSeg edges[12]) const;

  // Checks if point p intersects this obb. 
  // If so, returns true. Sets contactNormal to normal of nearest face to p. Sets pentration depth to
  //  positive distance from point to that nearest face.
  bool Intersects(const Vec3f& p, Vec3f* contactNormal, float* penetrationDepth) const;

  // Checks if edge e intersects this obb.
  // If so, returns true. Sets clip to segment of e that intersects box. Sets contactNormal to normal 
  //  of face nearest midpoint of clip segment. Sets penetration depth to positive dist from that face 
  //  to mid point of clip.
  bool Intersects(const LineSeg& e, LineSeg* clip, Vec3f* contactNormal, float* penDepth) const; 

private:
  Vec3f m_centre;
  Vec3f m_axes[3];
  Vec3f m_extents;
};

// Intersection test - SAT
bool Intersects(const OBB3& b1, const OBB3& b2);
}

#endif
