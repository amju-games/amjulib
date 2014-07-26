#ifndef DRAW_AABB_H
#define DRAW_AABB_H

#include <AABB.h>
#include <TriList.h>

namespace Amju
{
TriList* MakeAABBTriList(const AABB& a);

void DrawAABB(const AABB&);
void DrawSolidAABB(const AABB& a);
}

#endif

