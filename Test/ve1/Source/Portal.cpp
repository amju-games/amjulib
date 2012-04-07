#include "Portal.h"
#include <GameObjectFactory.h>
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "ObjectManager.h"

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
}

void Portal::OnLocationEntry()
{
  // TODO
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


