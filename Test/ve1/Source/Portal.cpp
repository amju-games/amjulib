#include "Portal.h"
#include <GameObjectFactory.h>
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "ObjectManager.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"

namespace Amju
{
const char* Portal::TYPENAME = "portal";
static const char* DEST_KEY = "dest_portal_id";

GameObject* CreatePortal()
{
  return new Portal;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Portal::TYPENAME, CreatePortal);

const char* Portal::GetTypeName() const
{
  return TYPENAME;
}

Portal::Portal()
{
  m_destLocation = 0;
  m_isOpen = false;

  Set(DEST_KEY, "1");
}

void Portal::Set(const std::string& key, const std::string& val)
{
  Ve1Object::Set(key, val);
  
  if (key == DEST_KEY)
  {
    // Set up reverse link TODO
//    PGameObject dest
  }
}

bool Portal::Load(File* f)
{
  // Load position, size of bounding box.
  // Load dest location.
  // NB All Ve1Objects know their own location

  return true;
}

AABB* Portal::GetAABB()
{
  return &m_aabb;
}

void Portal::Update()
{
  static const float XSIZE = 10.0f;
  static const float YSIZE = 20.0f;
  m_aabb.Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);

  if (m_sceneNode)
  {
    *(m_sceneNode->GetAABB()) = m_aabb;
  }
}

void Portal::OnLocationEntry()
{
  // TODO

  // Add node to Scene Graph
  m_sceneNode = new Ve1Node(this);
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->AddChild(m_sceneNode);
}

void Portal::OnPlayerCollision(Player* player)
{
  if (!m_isOpen)
  {
    return;
  }

  if (!player->IsLocalPlayer())
  {
    return;
  }

  // Change location

//  player->SetPos(m_destPos);

  TheObjectUpdater::Instance()->SendPosUpdateReq(player->GetId(), m_destPos, m_destLocation);

  // Should Player do this ?
  SetLocalPlayerLocation(m_destLocation); // LocalPlayer
}
}


