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
  virtual bool Load(File*);
  virtual void OnLocationEntry();
  virtual void SetKeyVal(const std::string&, const std::string&);

  void OnPlayerCollision(Player* player);

  virtual void SetEditMenu(GuiMenu*) {}

private:
  bool m_isOpen;
};

// Keep set of portals in current room, for Camera to search.
typedef std::set<RCPtr<Portal> > Portals;

Portals& GetPortals();
}

#endif

