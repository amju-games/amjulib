#include <GameObjectFactory.h>
#include <Game.h>
#include <Timer.h>
#include <SoundManager.h>
#include "TutorialNpc.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "GameMode.h"
#include "LurkMsg.h"
#include "GSCogTestMenu.h"
#include "ROConfig.h"

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
  m_ctState = CT_START;
  m_hasTriggered = false;

  // Prevent false triggers - doesn't work, pos is set later
//  SetPos(Vec3f(20000.0f, 0, 0)); // TODO CONFIG ?
//  MoveTo(GetPos()); 
}

void TutorialNpc::Update()
{
  Ve1ObjectChar::Update();

  m_timer += TheTimer::Instance()->GetDt();

  // Chase player if it's important to give this task
  if (TheGSCogTestMenu::Instance()->IsDoingTests())
  {
    switch (m_ctState)
    {
    case CT_START:
      {
        // Chase local player so we are visible and triggered
        Vec3f pos = GetLocalPlayer()->GetPos();
        MoveTo(pos + Vec3f(30.0f, 0, 0)); // TODO CONFIG
      }
      break;

    case CT_DOING_TESTS:
      {
        Vec3f pos = GetLocalPlayer()->GetPos();
        static const float MAX_FOLLOW_DIST = ROConfig()->GetFloat("npc-max-follow-dist", 200.0f);
        if ((pos - m_pos).SqLen() > MAX_FOLLOW_DIST * MAX_FOLLOW_DIST) 
        {
          MoveTo(pos + Vec3f(30.0f, 0, 0)); // TODO CONFIG
        }     
        // After a delay we trigger again
        static const float NPC_TRIGGER_DELAY = ROConfig()->GetFloat("npc-trigger-delay", 10.0f);
        if (m_timer > NPC_TRIGGER_DELAY) 
        {
          m_hasTriggered = false; // causing Trigger to be called again when we are close
          m_timer = 0;
        }
      }
      break;
    }
  }

  // Trigger if close enough to player and not already triggered
  if (!m_hasTriggered)
  {
    Vec3f p0 = m_pos;
    p0.y = 0;
    Vec3f p1 = GetLocalPlayer()->GetPos();
    p1.y = 0;
    static const float TRIGGER_RADIUS = ROConfig()->GetFloat("npc-max-follow-dist", 75.0f);
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

  // Start near player
  Vec3f pos = GetLocalPlayer()->GetPos();
  SetPos(pos + Vec3f(200.0f, 0, -30.0f)); // TODO CONFIG

  m_timer = 0;

  //if (DoCogTests() && !m_hasDoneCogTests) // && type=="cogtest"
  //{
  //  // Go next to local player so we are visible and triggered
  //  Vec3f pos = GetLocalPlayer()->GetPos();
  //  SetPos(pos + Vec3f(200.0f, 0, -30.0f)); // TODO CONFIG
  //  MoveTo(pos + Vec3f(30.0f, 0, 0)); // TODO CONFIG
  //}
}

void OnCogTestMsgFinished()
{
  //m_timer = 0;
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

void TutorialNpc::Trigger()
{
  TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-npc-trigger"));
  m_hasTriggered = true;

  // Turn to face player
  SetDirToFace(GetLocalPlayer());

  // Get player's token for this NPC if any


  // Is there a currently active objective ?

  // Has player achieved current objective ?

  // No - redisplay objective question

  // Decide on "question" depending on token

  
  if (m_ctState == CT_START) 
  {
    std::string text = "Hello " + GetLocalPlayer()->GetName() + 
      "!\nI have got a quiz for you. You will win lots of hearts by doing it!";
    // When this msg has been displayed, we go to the Cog Test state.
    LurkMsg lm(text, Colour(1, 1, 1, 1), Colour(0.5f, 0, 0.5f, 0.5f), AMJU_CENTRE, 
      OnCogTestMsgFinished);
    TheLurker::Instance()->Queue(lm);
    m_ctState = CT_DOING_TESTS;
    m_timer = 0;
  }
  else if (m_ctState == CT_DOING_TESTS)
  {
    TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
  }
}

}
