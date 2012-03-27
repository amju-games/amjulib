#ifndef PICK_OBJECT_H_INCLUDED
#define PICK_OBJECT_H_INCLUDED

#include <Vec2.h>

namespace Amju
{
class GameObject;

// Returns closest GameObject under mouse coord, or 0.
// NB mouseCoord is in normalised coords, -1..1 in x and y.
GameObject* PickObject(const Vec2f& mouseCoord);
}

#endif

