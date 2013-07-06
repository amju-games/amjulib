#include <AmjuFirst.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include <LoadVec3.h>
#include <LoadVec2.h>
#include <Timer.h>
#include "Baddie.h"
#include "BaddieNode.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "AttackEffect.h"
#include "CommandFight.h"
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
GameObject* CreateBaddie()
{
  return new Baddie;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Baddie::TYPENAME, CreateBaddie);

// TODO
static const float XSIZE = 20.0f;
static const float YSIZE = 5.0f;

const char* Baddie::TYPENAME = "baddie";

const char* Baddie::GetTypeName() const
{
  return TYPENAME;
}

Baddie::Baddie() 
{
  m_damage = 0;
  m_chaseSpeed = 0;
  m_isDestructible = true;
  m_maxHealth = 1; 
  m_health = m_maxHealth;

}

void Baddie::Update()
{
  Ve1Object::Update();

  // Surely this is all done in base class ???
  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
  }

  if (GetAABB())
  {
    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);

    //if (m_effect)
    //{
    //  *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    //}
  }
}

bool Baddie::Load(File* f)
{
  static int id = BADDDIE_START_ID;
  SetId(id++);

  Vec3f pos;
  if (!LoadVec3(f, &pos))
  {
    f->ReportError("Expected position");
    return false;
  }
  SetPos(pos);

  std::string tex;
  if (!f->GetDataLine(&tex))
  {
    f->ReportError("Expected texture filename");
    return false;
  }
  Vec2i cells;
  if (!LoadVec2(f, &cells))
  {
    f->ReportError("Expected num cells Vec2");
    return false;
  }
  Vec2f size;
  if (!LoadVec2(f, &size))
  {
    f->ReportError("Expected size Vec2");
    return false;
  }
  if (!f->GetInteger(&m_damage))
  {
    f->ReportError("Expected damage");
    return false;
  }

  // TODO Load max health

  m_maxHealth = 10; 
  m_health = m_maxHealth;

  BaddieNode* bn = new BaddieNode(this, tex, cells.x, cells.y, size.x * 0.5f, size.y * 0.5f);

  SetSceneNode(bn);

  /*
  if (!m_shadow->Load(f))
  {
    return false;
  }

  // Particle effect when attacked, etc.
  File fight;
  if (!fight.OpenRead("fighteffect.txt"))
  {
    fight.ReportError("Failed to load effect");
    return false;
  }

  m_effect = new AttackEffect;
  if (!m_effect->Load(&fight))
  {
    fight.ReportError("Failed to load effect");
    return false;
  }
  m_effect->SetVisible(true);

  m_sceneNode->AddChild(m_effect.GetPtr());
  */

  return true;
}

void Baddie::SetMenu(GuiMenu* menu)
{
  // No menu, but fire at it

  Player* p = GetLocalPlayer();
  if (p)
  {
    p->ShootBaddie(this);
  }

  //menu->AddChild(new GuiMenuItem("FIGHT!!", new CommandFight(this))); 
////  AddMenuItem("FIGHT!!", new CommandFight(this)); 
}

void Baddie::OnCollideBullet()
{
  // Take damage - destroy if health <= 0
  m_health--;

  float f = (float)m_health / (float)m_maxHealth;
  Colour col(f, f, f, 1);
  m_sceneNode->SetColour(col);

  if (m_health <= 0)
  {
    OnLocationExit();
    TheGame::Instance()->EraseGameObject(GetId());
  }
}


ToggleBaddie::ToggleBaddie()
{
  m_harmful = false;
  for (int i = 0; i < 4; i++)
  {
    m_time[i] = 0;
    m_cellRange[i] = 0;
  }
  m_timeInState = 0;
  m_maxTimeInState = 0;
}

void ToggleBaddie::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  m_timeInState += dt;
  if (m_timeInState > m_maxTimeInState)
  {
    m_timeInState = 0;
    m_harmful = !m_harmful;
    // New random time, within range for this state
  }
}

int ToggleBaddie::GetDamage() const
{
  return m_harmful ? m_damage : 0;
}

bool ToggleBaddie::Load(File* f)
{
  if (!Baddie::Load(f))
  {
    return false;
  }

  return true;
}
}


