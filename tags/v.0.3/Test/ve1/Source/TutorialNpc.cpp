#include <GameObjectFactory.h>
#include "TutorialNpc.h"
#include "Ve1SceneGraph.h"

namespace Amju
{
const char* TutorialNpc::TYPENAME = "tutorial_npc";

GameObject* CreateTutorialNpc()
{
  return new TutorialNpc;
}

static bool registered = TheGameObjectFactory::Instance()->Add(TutorialNpc::TYPENAME, CreateTutorialNpc);

void TutorialNpc::Update()
{
  Ve1ObjectChar::Update();
}

bool TutorialNpc::Load(File* f)
{
  Ve1Character* node = new Ve1Character;

  File dinoFile;
  if (!dinoFile.OpenRead("tutorial-npc.txt"))
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

/*
  Ve1Character* psn = new Ve1Character; 
  m_sceneNode = psn;

  if (!psn->Load(f))
  {
    return false;
  }

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());
*/

  // TODO Load script

  return true;
}

void TutorialNpc::SetMenu(GuiMenu*)
{
  // Talk - gives objectives, tutorial, etc.
}

}
