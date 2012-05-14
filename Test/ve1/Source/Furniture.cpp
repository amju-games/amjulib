#include <GameObjectFactory.h>
#include "Furniture.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "GameMode.h"

namespace Amju
{
const char* Furniture::TYPENAME = "furniture";
  
static const float XSIZE = 20.0f; // TODO CONFIG
static const float YSIZE = 40.0f;

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
  AABB aabb;
  aabb.Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);

  if (m_sceneNode)
  {
    *(m_sceneNode->GetAABB()) = aabb;
  }

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

  AABB aabb;
  aabb.Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);

  //if (GetGameMode() == AMJU_MODE_EDIT)
  {
    // Add node to Scene Graph
    m_sceneNode = new Ve1Node(this);
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->AddChild(m_sceneNode);
    *(m_sceneNode->GetAABB()) = aabb;
  }

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

