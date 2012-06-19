#ifndef PORTAL_H_INCLUDED
#define PORTAL_H_INCLUDED

#include <SceneNode.h>
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

  // For collision testing 
  AABB* GetAABB();

private:
  bool m_isOpen;
  AABB m_aabb;
  RCPtr<SceneNode> m_sceneNode;
};
}

#endif

