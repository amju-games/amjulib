#ifndef FURNITURE_H_INCLUDED
#define FURNITURE_H_INCLUDED

#include <SceneMesh.h>
#include <Shadow.h>
#include "Ve1Object.h"
#include "HasCollisionMesh.h"

namespace Amju
{
class Player;

// Visible, collidable objects which don't move autonomously.
// But they can be carried by players, repositioned, and moved to other locations.
class Furniture : public Ve1Object, public HasCollisionMesh
{
public:
  static const char* TYPENAME;

  Furniture();
  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual bool Load(File*);
  virtual void SetKeyVal(const std::string& key, const std::string& val);
  virtual void SetMenu(GuiMenu*);
  virtual void SetEditMenu(GuiMenu*);

  void OnPlayerCollision(Player* player);

  AABB* GetAABB(); // could be zero if picked up, etc.

  virtual CollisionMesh* GetCollisionMesh();

protected:
  int m_pickupId;
};
}

#endif

