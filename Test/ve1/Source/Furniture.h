#ifndef FURNITURE_H_INCLUDED
#define FURNITURE_H_INCLUDED

#include <SceneMesh.h>
#include <Shadow.h>
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
  virtual void OnLocationExit();
  virtual void SetKeyVal(const std::string& key, const std::string& val);
  virtual void SetMenu(GuiMenu*);
  
  void OnPlayerCollision(Player* player);

  // For collision testing 
  AABB* GetAABB();

private:
  AABB m_aabb; // Not needed, can use SceneNode AABB ?
  RCPtr<SceneNode> m_sceneNode;

  RCPtr<Shadow> m_shadow;
};
}

#endif

