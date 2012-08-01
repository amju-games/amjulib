#include <GameObjectFactory.h>
#include "TutorialNpc.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"

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
  Ve1Character* node = new Ve1Character(this);

  m_sceneNode = node;

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());

  m_inNewLocation = true;

  return true;
}

void TutorialNpc::SetMenu(GuiMenu*)
{
  // Talk - gives objectives, tutorial, etc.
}

}
