#include <AmjuFirst.h>
#include <Timer.h>
#include <DegRad.h>
#include "Ve1Object.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "Ve1Node.h"
#include "GameMode.h"
#include "Ve1SceneGraph.h"
#include "Useful.h"
#include "HasCollisionMesh.h"
#include "Terrain.h"
#include "ObjectUpdater.h"
#include "ROConfig.h"
#include "GSMain.h"
#include "Terrain.h"
#include <AmjuFinal.h>

//#define LOCATION_DEBUG

namespace Amju
{
static const float BOUNCE_VEL = -50.0f;

static const char* HEADING_KEY = "heading";

Ve1Object::Ve1Object() : m_location(-1)
{
  m_isSelected = false;
  m_ignorePortalId = -1;
  m_isPickable = true;
  m_hidden = false;
  m_collidingObject = 0;
  m_shadow = new Shadow;
  m_shadow->SetHeightRange(10.0f, 25.0f); // TODO TEMP TEST CONFIG
  m_inNewLocation = 2;
  m_dir = 0;
  m_dirCurrent = m_dir;
  m_capsuleRadius = 30.0f; // Default, get better value in subclasses
}

Ve1Object::~Ve1Object()
{
  OnLocationExit();
}

void Ve1Object::AddMenuItem(const std::string& text, GuiCommand* command)
{
  // TODO Pass on to GSMain
  TheGSMain::Instance()->AddMenuItem(text, command);
}

bool Ve1Object::IsHidden() const
{
  return m_hidden;
}

void Ve1Object::SetHidden(bool b)
{
  m_hidden = b;

  if (m_sceneNode)
  {
    m_sceneNode->SetVisible(!m_hidden);
  }
}

void Ve1Object::SetSceneNode(SceneNode* n)
{
  // Remove any existing node from scene graph
  Ve1Object::OnLocationExit();

  m_sceneNode = n;
}

void Ve1Object::Update()
{
  m_oldPos = m_pos;

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
#ifdef LOCATION_DEBUG
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
#ifdef LOCATION_DEBUG
std::cout << "Setting location for " << m_id << " (" << GetTypeName() << "): " << newLocation << " (old: " << m_location << ")...\n";
#endif

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
  // BUT if selected, we are moving it in edit mode, so ignore where the server thinks we are
  if (!m_isSelected)
  {
    SetPos(pos);
  }
}

void Ve1Object::SetKeyVal(const std::string& key, const std::string& val)
{
  m_valmap[key] = val; 

  if (key == "hidden")
  {
    m_hidden = (val == "y");
  }
  else if (key == HEADING_KEY) 
  {
    if (this != GetLocalPlayer())
    {
      // We sent this to server a while ago
      m_dir = ToFloat(val); 
    }
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
    if (root)
    {
      root->AddChild(m_sceneNode);
      m_sceneNode->SetVisible(!m_hidden);
    }
  }
}

void Ve1Object::OnLocationExit()
{
  // Remove from SceneGraph
  if (m_sceneNode)
  {
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    if (root)
    {
      root->DelChild(m_sceneNode.GetPtr());
    }
  }
}

}

