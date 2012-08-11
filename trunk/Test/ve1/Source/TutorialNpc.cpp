#include <GameObjectFactory.h>
#include <Game.h>
#include "TutorialNpc.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "GameMode.h"
#include "LurkMsg.h"
#include "GSCogTestMenu.h"

namespace Amju
{
const char* TutorialNpc::TYPENAME = "tutorial_npc";

GameObject* CreateTutorialNpc()
{
  return new TutorialNpc;
}

static bool registered = TheGameObjectFactory::Instance()->Add(TutorialNpc::TYPENAME, CreateTutorialNpc);

TutorialNpc::TutorialNpc()
{
  m_hasDoneCogTests = false;
  m_hasTriggered = false;
}

void TutorialNpc::Update()
{
  Ve1ObjectChar::Update();

  // Chase player if it's important to give this task
  if (DoCogTests() && !m_hasDoneCogTests) // && type=="cogtest"
  {
    // Go next to local player so we are visible and triggered
    Vec3f pos = GetLocalPlayer()->GetPos();
    MoveTo(pos + Vec3f(30.0f, 0, 0)); // TODO CONFIG
  }

  // Trigger if close enough to player and not already triggered
  if (!m_hasTriggered)
  {
    Vec3f p0 = m_pos;
    p0.y = 0;
    Vec3f p1 = GetLocalPlayer()->GetPos();
    p1.y = 0;
    static const float TRIGGER_RADIUS = 75.0f; // TODO CONFIG
    static const float TR2 = TRIGGER_RADIUS * TRIGGER_RADIUS;
    if ((p1 - p0).SqLen() < TR2)
    {
      SetVel(Vec3f()); // make sure we stop
      Trigger();
    }
  }
}

bool TutorialNpc::Load(File* f)
{
  Ve1Character* node = new Ve1Character(this);

  m_sceneNode = node;

  if (!m_shadow->Load(f))
  {
    return false;
  }

  m_hasTriggered = false;
  return true;
}

void TutorialNpc::SetMenu(GuiMenu*)
{
  // Talk - gives objectives, tutorial, etc.
}

void TutorialNpc::OnPlayerCollision(Player* player)
{
  // Already triggered in Update, when within some radius of player
}

void TutorialNpc::OnLocationEntry()
{
  Ve1ObjectChar::OnLocationEntry();

  // TODO Special types of TutNPCs, depends on key

  if (DoCogTests() && !m_hasDoneCogTests) // && type=="cogtest"
  {
    // Go next to local player so we are visible and triggered
    Vec3f pos = GetLocalPlayer()->GetPos();
    SetPos(pos + Vec3f(200.0f, 0, -30.0f)); // TODO CONFIG
    MoveTo(pos + Vec3f(30.0f, 0, 0)); // TODO CONFIG
  }
}

void OnCogTestMsgFinished()
{
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

void TutorialNpc::Trigger()
{
  m_hasTriggered = true;

  // Turn to face player
  SetDirToFace(GetLocalPlayer());

  // Get player's token for this NPC if any


  // Is there a currently active objective ?

  // Has player achieved current objective ?

  // No - redisplay objective question

  // Decide on "question" depending on token

  // Show "question" text
  std::string text = "Hello " + GetLocalPlayer()->GetName() + 
    "!\nI have got a quiz for you. You will win lots of jellybeans by doing it!";
  // When this msg has been displayed, we go to the Cog Test state.
  LurkMsg lm(text, Colour(1, 1, 1, 1), Colour(0.5f, 0, 0.5f, 0.5f), AMJU_CENTRE, 
    OnCogTestMsgFinished);
  TheLurker::Instance()->Queue(lm);

  if (DoCogTests()) 
  {
    m_hasDoneCogTests = true;
  }
}

}
