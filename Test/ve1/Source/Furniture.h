#ifndef FURNITURE_H_INCLUDED
#define FURNITURE_H_INCLUDED

#include <SceneNode.h>
#include "Ve1Object.h"

namespace Amju
{
class Player;

// Visible, collidable objects which don't move autonomously.
// But they can be carried by players, repositioned, and moved to other locations.
class Furniture : public Ve1Object
{
public:
  static const char* TYPENAME;

  Furniture();
  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual bool Load(File*);
  virtual void OnLocationEntry();
  virtual void Set(const std::string&, const std::string&);

  void OnPlayerCollision(Player* player);

  // For collision testing 
  AABB* GetAABB();

private:
  AABB m_aabb; // Not needed, can use SceneNode AABB ?
  RCPtr<SceneNode> m_sceneNode;
};
}

#endif

