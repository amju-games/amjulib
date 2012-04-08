#include "Portal.h"
#include <GameObjectFactory.h>
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "ObjectManager.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "Useful.h"

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
  m_isOpen = true;

  Set(DEST_KEY, "1"); // value is game object ID for destination portal
}

void Portal::Set(const std::string& key, const std::string& val)
{
  Ve1Object::Set(key, val);
  
  if (key == DEST_KEY)
  {
    // Set up reverse link 
    int destId = ToInt(val);
    Ve1Object* destObj = dynamic_cast<Ve1Object*>(TheObjectManager::Instance()->GetGameObject(destId).GetPtr());
    if (!destObj)
    {
std::cout << "* Portal: dest portal doesn't exist [yet]..\n";
      return ;
    }
    
    std::string thisId = ToString(GetId());
    if (destObj->GetVal(DEST_KEY) != thisId)
    {
std::cout << "Setting dest ID " << thisId << " for portal " << destId << "\n";
      destObj->Set(DEST_KEY, thisId);
      // TODO Should we send this update to server ? Or just always set it in client!?
      TheObjectUpdater::Instance()->SendUpdateReq(destObj->GetId(), DEST_KEY, thisId);
    }
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
  static const float XSIZE = 20.0f; // TODO CONFIG
  static const float YSIZE = 40.0f;
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

  // TODO Carried objects

  if (!player->IsLocalPlayer())
  {
    return;
  }

  // Change location
  if (!Exists(DEST_KEY))
  {
std::cout << "* Portal: Dest key doesn't exist!\n";
    return; 
  }

  if (player->GetIgnorePortalId() == this->GetId())
  {
std::cout << "* Portal: Set to ignore this one.\n";
    return;
  }

  std::string strId = GetVal(DEST_KEY);
  int destId = ToInt(strId);

std::cout << "* Entered portal! Destination portal ID is " << destId << "...\n";
  Ve1Object* destObj = dynamic_cast<Ve1Object*>(TheObjectManager::Instance()->GetGameObject(destId).GetPtr());
  if (!destObj)
  {
std::cout << "* Portal: dest portal doesn't exist [yet]..\n";
    return ;
  }
  int destLocation = destObj->GetLocation();
  const Vec3f& destPos = destObj->GetPos();
std::cout << "* Dest location: " << destLocation << " dest pos: " << destPos << "\n";

  TheObjectUpdater::Instance()->SendPosUpdateReq(player->GetId(), destPos, destLocation);

  // Should Player do this ?
  SetLocalPlayerLocation(destLocation); // LocalPlayer

  // Set new position immediately
  player->SetPos(destPos);
  // Move in current direction a little bit ?
  Vec3f vel = player->GetVel();
  vel.Normalise();
  vel *= 100.0f;
  player->MoveTo(destPos + vel);

  // Set dest portal so we don't flip back and forth
  Portal* destPortal = dynamic_cast<Portal*>(destObj);
  Assert(destPortal);
  player->IgnorePortalId(destId);
}
}


