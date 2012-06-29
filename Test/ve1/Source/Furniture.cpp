#include <GameObjectFactory.h>
#include "Furniture.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "GameMode.h"
#include "Player.h"

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
  Ve1Object::Update();

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
  // Create Scene Node, but don't attach to SceneGraph until needed

  // Get mesh filename. 
  std::string meshFilename;
  if (!f->GetDataLine(&meshFilename))
  {
    f->ReportError("Expected mesh file name for furniture");
    return false;
  }

  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(meshFilename);
  Assert(mesh);
  SceneMesh* sm = new SceneMesh;
  sm->SetMesh(mesh);
  m_sceneNode = sm;

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());
  return true;
}

void Furniture::OnLocationEntry()
{
  // TODO Base class

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

void Furniture::OnLocationExit()
{
  // TODO Base class
  // Remove from SceneGraph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->DelChild(m_sceneNode.GetPtr());
}

void Furniture::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);
}

void Furniture::SetMenu(GuiMenu* menu)
{
  // If not currently carried; if you are strong enough
//  menu->AddChild(new GuiMenuItem("Pick up", new CommandPickUp(this)));
}

void Furniture::OnPlayerCollision(Player* player)
{
  // We might get pushed away from player, or player could move back -- depends on our "mass".

//std::cout << "Guhhhh!!! Collide with furniture!! player: " << player->GetId() << " furniture: " << GetId() << "\n";
}

AABB* Furniture::GetAABB()
{
  if (!m_sceneNode) return 0;
  return m_sceneNode->GetAABB();
}
}

