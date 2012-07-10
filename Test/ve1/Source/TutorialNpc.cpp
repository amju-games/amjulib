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
}

bool TutorialNpc::Load(File* f)
{
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

  // TODO Load script

  return true;
}

void TutorialNpc::OnLocationEntry()
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->AddChild(m_sceneNode.GetPtr());
}

void TutorialNpc::OnLocationExit()
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->DelChild(m_sceneNode.GetPtr());
}

void TutorialNpc::SetKeyVal(const std::string& key, const std::string& val)
{
}

void TutorialNpc::SetMenu(GuiMenu*)
{
  // Talk - gives objectives, tutorial, etc.
}

}
