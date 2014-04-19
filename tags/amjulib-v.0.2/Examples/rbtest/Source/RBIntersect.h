#ifndef RB_INTERSECT_H
#define RB_INTERSECT_H

namespace Amju
{
class RBSphere;

bool Intersects(const RBSphere& s1, const RBSphere& s2, float* timeFirstContact);
}

#endif
