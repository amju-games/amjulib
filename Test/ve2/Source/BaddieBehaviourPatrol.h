#ifndef BB_PATROL_H_INCLUDED
#define BB_PATROL_H_INCLUDED

#include <Rect.h>
#include "Baddie.h"

namespace Amju
{
class BBPatrol : public BaddieBehaviour
{
public:
  static const char* TYPENAME;

  BBPatrol();
  virtual void Update();
  virtual bool Load(File*);

protected:
  // Patrol direction (velocity vector in x,z)
  Vec2f m_patrolDir;
  // Bounding box
  Rect m_rect;
  // Reverse direction when we reach the edge of the box, or wrap around to the other side?
  bool m_reverse;
};
}

#endif

