#include "Ve1Object.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "Ve1Node.h"
#include "GameMode.h"
#include "Ve1SceneGraph.h"
#include "Useful.h"

namespace Amju
{
Ve1Object::Ve1Object() : m_location(-1)
{
  m_isSelected = false;
  m_ignorePortalId = -1;
  m_isPickable = true;
  m_hidden = false;
}

bool Ve1Object::IsHidden() const
{
  return m_hidden;
}

AABB* Ve1Object::GetAABB()
{
  return &m_aabb;
}

void Ve1Object::SetEditMenu(GuiMenu* menu)
{
}

void Ve1Object::CreateEditNode()
{
  if (GetGameMode() == AMJU_MODE_EDIT && !m_sceneNode)
  {
    static const float XSIZE = 20.0f; // TODO CONFIG
    static const float YSIZE = 40.0f;

    m_sceneNode = new Ve1Node(this);
    m_aabb.Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);
    *(m_sceneNode->GetAABB()) = m_aabb;

    SceneNode* name = new Ve1NameNode(this);
    AABB aabb(-10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f);
    *(name->GetAABB()) = aabb;
    m_sceneNode->AddChild(name);
  }
}

void Ve1Object::Update()
{
  m_oldPos = m_pos;

  if (m_sceneNode)
  {
    m_sceneNode->SetVisible(!m_hidden);
  }

  if (!IsHidden())
  {
    GameObject::Update();
  }
}

const Vec3f& Ve1Object::GetOldPos() const
{
  return m_oldPos;
}

bool Ve1Object::IsPickable() const 
{ 
  return m_isPickable; 
}

void Ve1Object::IgnorePortalId(int portalId)
{
  m_ignorePortalId = portalId;
}

int Ve1Object::GetIgnorePortalId() const
{
  return m_ignorePortalId;
}

bool Ve1Object::IsSelected() const
{
  return m_isSelected;
}

void Ve1Object::SetSelected(bool selected)
{
  m_isSelected = selected;
}

int Ve1Object::GetLocation() const
{
#ifdef _DEBUG
  if (m_location == -1)
  {
    std::cout << "Warning, object " << GetId() << " has location -1.\n"; 
  }
#endif

  //Assert(m_location != -1);

  return m_location;
}

void Ve1Object::SetLocation(int newLocation)
{
std::cout << "Setting location for " << m_id << " (" << GetTypeName() << "): " << newLocation << " (old: " << m_location << ")...\n";

  // Remove from game & scene graph etc if not in same location as local player
  // BUT object could also Enter the local player location!

  int oldLocation = m_location;

  if (oldLocation == newLocation)
  {
    return;
  }

  m_location = newLocation;

  int localPlayerLoc = GetLocalPlayerLocation();
  
  if (oldLocation == localPlayerLoc && newLocation != localPlayerLoc)
  {
    // We have just exited the local player location
    //OnLocationExit();  // ObjectManager will call, right ?
    // TODO notify ObjectManager ?
    TheObjectManager::Instance()->OnObjectChangeLocation(m_id); 
  }
  else if (oldLocation != localPlayerLoc && newLocation == localPlayerLoc)
  {
    // We have just entered the local player location
    //OnLocationEntry();
    TheObjectManager::Instance()->OnObjectChangeLocation(m_id); 
  }

  // NB What about when an object gets a Move or other change to state, and is not in the local
  //  player location...??!?!
  // It still gets updates if it exists in memory (i.e. ObjectManager has created it)
}

void Ve1Object::MoveTo(const Vec3f& pos)
{
  // Default behaviour: instantly change position
  SetPos(pos);
}

void Ve1Object::SetKeyVal(const std::string& key, const std::string& val)
{
  m_valmap[key] = val; 

  if (key == "hidden")
  {
std::cout << "Setting hidden flag for " << *this << " to: " << val << "\n";
    m_hidden = (val == "y");
  }
}

bool Ve1Object::Exists(const std::string& key) const
{
  return (m_valmap.find(key) != m_valmap.end());
}

const std::string& Ve1Object::GetVal(const std::string& key) const
{
  Assert(Exists(key));
  ValMap::const_iterator it = m_valmap.find(key);
  Assert(it != m_valmap.end());
  return it->second;
}

ValMap* Ve1Object::GetValMap()
{
  return &m_valmap;
}

void Ve1Object::OnLocationEntry()
{
  if (m_sceneNode)
  {
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->AddChild(m_sceneNode);
  }
}

void Ve1Object::OnLocationExit()
{
  // Remove from SceneGraph
  if (m_sceneNode)
  {
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->DelChild(m_sceneNode.GetPtr());
  }
}

}

