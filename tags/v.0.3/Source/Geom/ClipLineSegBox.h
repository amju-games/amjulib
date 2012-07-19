#ifndef CLIP_LINE_SEG_BOX_H
#define CLIP_LINE_SEG_BOX_H

#include "Line3.h"
#include "AABB.h"
#include "OBB3.h"

namespace Amju
{
// Clips the given line segment against the given bounding box.
// Returns false if the line seg is completely clipped, i.e. there is no intersection.
// Returns true if the line seg and box intersect, i.e. all or some of the line seg is left after clipping.
// The line seg pointed to by clipped will be set to the clipped
//  remnant of the original line seg, if clipped is not zero.

bool Clip(const LineSeg& lineSeg, const AABB& box, LineSeg* clipped = 0);

bool Clip(const LineSeg& lineSeg, const OBB3& box, LineSeg* clipped = 0);
}

#endif
