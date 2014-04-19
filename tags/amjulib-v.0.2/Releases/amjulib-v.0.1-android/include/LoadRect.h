#ifndef LOAD_RECT_H_INCLUDED
#define LOAD_RECT_H_INCLUDED

#include <Rect.h>

namespace Amju
{
class File;

// Expects line of format: minx, maxx, miny, maxy. E.g. 
// 1, 2, 3, 4
// ..is a rect from (1, 3) to (2, 4)
bool LoadRect(File* f, Rect* r);
}

#endif
