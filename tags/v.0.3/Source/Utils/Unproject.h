#ifndef AMJU_UNPROJECT_H_INCLUDED
#define AMJU_UNPROJECT_H_INCLUDED

// TODO Move to amjulib. AmjuGL or Geom ?
// Depends on AmjuGL for matrices and Viewport

#include "Vec2.h"
#include "Vec3.h"

namespace Amju
{
bool Unproject(const Vec2f& screenPos, float z, Vec3f* result);

bool Project(const Vec3f& pos, Vec2f* result);
}

#endif
