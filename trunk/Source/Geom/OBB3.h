#ifndef OBB3_H
#define OBB3_H

#include <Vec3.h>
#include <Quaternion.h>
#include <Plane.h>

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

  bool Intersects(const Vec3f& p, Vec3f* contactNormal, float* penetrationDepth) const;

private:
  Vec3f m_centre;
  Vec3f m_axes[3];
  Vec3f m_extents;
};

// Intersection test - SAT
bool Intersects(const OBB3& b1, const OBB3& b2);
}

#endif
