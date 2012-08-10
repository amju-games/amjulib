#include <GameObjectFactory.h>
#include "TutorialNpc.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "GameMode.h"

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

  if (!m_shadow->Load(f))
  {
    return false;
  }

  return true;
}

void TutorialNpc::SetMenu(GuiMenu*)
{
  // Talk - gives objectives, tutorial, etc.
}

void TutorialNpc::OnPlayerCollision(Player* player)
{
  if (player->IsLocalPlayer())
  {
    Trigger();
  }
}

void TutorialNpc::OnLocationEntry()
{
  // TODO Special types of TutNPCs, depends on key

  if (DoCogTests()) // && type=="cogtest"
  {
    // Go next to local player so we are visible and triggered

    Vec3f pos = GetLocalPlayer()->GetPos();
    pos.x += 50.0f;
    SetPos(pos);
    Trigger();
  }
}

void TutorialNpc::Trigger()
{
  // Turn to face player

  // Get player's token for this NPC if any


  // Is there a currently active objective ?

  // Has player achieved current objective ?

  // No - redisplay objective question

  // Decide on "question" depending on token

  // Show "question" text
}

}
