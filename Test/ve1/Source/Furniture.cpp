#include <GameObjectFactory.h>
#include "Furniture.h"

namespace Amju
{
const char* Furniture::TYPENAME = "furniture";

GameObject* CreateFurniture()
{
  return new Furniture;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Furniture::TYPENAME, CreateFurniture);

Furniture::Furniture()
{
  // Set pos to out of range, so invisible until pos set
}

const char* Furniture::GetTypeName() const
{
  return TYPENAME;
}

void Furniture::Update()
{
  // If we have moved, respond to collisions with walls; get height; set AABB

  // If moving and like a ball, we should rotate
}

bool Furniture::Load(File* f)
{
  // Get mesh filename. 

  // Load shadow

  return true;
}

void Furniture::OnLocationEntry()
{
  // Set scene node

  // Set AABB
}

void Furniture::Set(const std::string& key, const std::string& val)
{
  Ve1Object::Set(key, val);
}

void Furniture::OnPlayerCollision(Player* player)
{
  // We might get pushed away from player, or player could move back -- depends on our "mass".

  // 
}

AABB* Furniture::GetAABB()
{
  if (!m_sceneNode) return 0;
  return m_sceneNode->GetAABB();
}
}

