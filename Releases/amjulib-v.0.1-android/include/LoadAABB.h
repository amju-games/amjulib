#ifndef LOAD_AABB_H
#define LOAD_AABB_H

namespace Amju
{
class File;
class AABB;

bool LoadAABB(File*, AABB*);
bool SaveAABB(File*, const AABB&);
}

#endif
