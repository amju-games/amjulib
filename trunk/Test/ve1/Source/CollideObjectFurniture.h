#ifndef COLLIDE_OBJ_FURNITURE_H_INCLUDED
#define COLLIDE_OBJ_FURNITURE_H_INCLUDED

namespace Amju
{
class GameObject;

// Collision between Ve1Object and an immovable object, e.g. Furniture.
// go1 is pushed back. go2 doesn't move.
void CollideObjectFurniture(GameObject* go1, GameObject* go2);
}

#endif

