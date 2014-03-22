#ifndef LOAD_VEC2_H
#define LOAD_VEC2_H

#include "Vec2.h"

namespace Amju
{
class File;

bool LoadVec2(File* f, Vec2f* v);

bool LoadVec2(File* f, Vec2i* v);
}

#endif

