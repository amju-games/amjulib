#ifndef PORTAL_H_INCLUDED
#define PORTAL_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
// If local player intersects portal, we are transported to another location
class Player;

class Portal : public Ve1Object
{
public:
  static const char* TYPENAME;

  Portal();
  virtual const char* GetTypeName() const;
  virtual void Update();

  virtual void OnLocationEntry();
  void OnPlayerCollision(Player* player);

private:
  int m_destLocation;
  Vec3f m_destPos;
  bool m_isOpen;
};
}

#endif

