#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "Ve1Object.h"
#include "HasCollisionMesh.h"
#include "Furniture.h"

namespace Amju
{
class Player;

class Building : public Furniture 
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  // TODO Should be virtual in base class
  void OnPlayerCollision(Player* player);

  // No menu items, so we can select positions inside
  virtual void SetMenu(GuiMenu*) {}

//  virtual void SetEditMenu(GuiMenu*);
//  virtual void SetKeyVal(const std::string& key, const std::string& val);
//  virtual CollisionMesh* GetCollisionMesh();
//  virtual bool Load(File*);
};
}

#endif

