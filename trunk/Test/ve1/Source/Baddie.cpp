#include <GameObjectFactory.h>
#include "Baddie.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"

namespace Amju
{
GameObject* CreateBaddie()
{
  return new Baddie;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Baddie::TYPENAME, CreateBaddie);


const char* Baddie::TYPENAME = "baddie";

const char* Baddie::GetTypeName() const
{
  return TYPENAME;
}

void Baddie::Update()
{
}

bool Baddie::Load(File* f)
{
  // Create Scene Node, but don't attach to SceneGraph until needed
  Ve1Character* node = new Ve1Character;
  m_sceneNode = node;

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());

  return true;
}

void Baddie::OnLocationExit()
{
  // Remove from SceneGraph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->DelChild(m_sceneNode.GetPtr());
}

void Baddie::OnLocationEntry()
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

std::cout << "Adding scene node to SceneGraph for baddie\n";

  root->AddChild(m_sceneNode.GetPtr());

  m_isMoving = false;
  SetVel(Vec3f(0, 0, 0)); 

  m_inNewLocation = true;
}

}


