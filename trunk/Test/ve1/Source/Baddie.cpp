#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
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
  Ve1ObjectChar::Update();
}

bool Baddie::Load(File* f)
{
std::cout << "Called baddie load function.\n";

  m_shadow = new Shadow;
//  m_shadow->SetSize(20.0f); // TODO TEMP TEST
  if (!m_shadow->Load(f))
  {
    return false;
  }

  return true;
}

void Baddie::OnLocationEntry()
{
  // Create Scene Node, but don't attach to SceneGraph until needed
  Ve1Character* node = new Ve1Character;

  File dinoFile;
  if (!dinoFile.OpenRead("dino.txt"))
  {
std::cout << "Baddie scene node: Failed to load dino.\n";
    Assert(0);
  }

  if (!node->Load(&dinoFile))
  {
std::cout << "Baddie scene node: Failed to load dino.\n";
    Assert(0); //return false;
  }

  m_sceneNode = node;

  m_shadow = new Shadow;
  m_shadow->SetSize(20.0f); // TODO 
  Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes("shadow.png"); // TODO

  m_shadow->SetTexture(tex);
  m_sceneNode->AddChild(m_shadow.GetPtr());

  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

std::cout << "Adding scene node to SceneGraph for baddie\n";

  root->AddChild(m_sceneNode.GetPtr());

  m_isMoving = false;
  SetVel(Vec3f(0, 0, 0)); 

  m_inNewLocation = true;
}

}


