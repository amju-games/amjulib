#include <AmjuFirst.h>
#include <SoundManager.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include <LoadVec3.h>
#include <LoadVec2.h>
#include <Timer.h>
#include <AmjuRand.h>
#include "Baddie.h"
#include "BaddieNode.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "AttackEffect.h"
#include "CommandFight.h"
#include "GameConsts.h"
#include "HeartCount.h"
#include <AmjuFinal.h>

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

Baddie::Baddie() 
{
  m_damage = 0;
  m_isDestructible = true;
  m_maxHealth = 1; 
  m_health = m_maxHealth;

  m_points = 0;
}

void Baddie::Update()
{
  Ve1Object::Update();

  if (m_bb)
  {
    m_bb->Update();
  }

  // Surely this is all done in base class ???
  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
  }

  if (GetAABB())
  {
    // Make bounding box smaller to give player the benefit of the doubt
    float x = m_size.x * 0.4f;
    float y = m_size.y * 0.4f;

    static const float YSIZE = 50.0f;

    GetAABB()->Set(
      m_pos.x - x, m_pos.x + x,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - y, m_pos.z + y);

    //if (m_effect)
    //{
    //  *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    //}
  }
}

Sprite& Baddie::GetSprite()
{
  Assert(m_sceneNode);
  Assert(dynamic_cast<SpriteNode*>(m_sceneNode.GetPtr()));
  return *(((SpriteNode*)m_sceneNode.GetPtr())->GetSprite());
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
  
  if (!LoadVec2(f, &m_size))
  {
    f->ReportError("Expected size Vec2");
    return false;
  }

  BaddieNode* bn = new BaddieNode(this, tex, cells.x, cells.y, m_size.x * 0.5f, m_size.y * 0.5f);
  // By default we just cycle through all cells
  bn->GetSprite()->SetCellRange(0, cells.x * cells.y);
  SetSceneNode(bn);

  if (!f->GetInteger(&m_damage))
  {
    f->ReportError("Expected damage");
    return false;
  }

  if (!f->GetInteger(&m_maxHealth))
  {
    f->ReportError("Expected max health");
    return false;
  }

  if (m_maxHealth == -1)
  {
    m_maxHealth = 1;
    m_isDestructible = false;
  }

  m_health = m_maxHealth;

  if (!f->GetInteger(&m_points))
  {
    f->ReportError("Expected num points");
    return false;
  }

  std::string bb;
  if (!f->GetDataLine(&bb))
  {
    f->ReportError("Expected baddie behaviour type");
    return false;
  }

  m_bb = TheBBFactory::Instance()->Create(bb);
  
  if (m_bb)
  {
    m_bb->SetBaddie(this);

    if (!m_bb->Load(f))
    {
      f->ReportError("Failed to load baddie behaviour");
      return false;
    }
  }

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
  // Don't try to shoot indestructible baddies..?
  if (!m_isDestructible)
  {
    return;
  }

  // No menu, but fire at this baddie
  Player* p = GetLocalPlayer();
  if (p)
  {
    p->ShootBaddie(this);
  }
}

void Baddie::OnCollideBullet()
{
  if (!m_isDestructible)
  {
    return;
  }

  // Take damage - destroy if health <= 0
  m_health--;

  float f = (float)m_health / (float)m_maxHealth;
  Colour col(f, f, f, 1);
  m_sceneNode->SetColour(col);

  if (m_health <= 0)
  {
    OnLocationExit();
    TheGame::Instance()->EraseGameObject(GetId());

    // Player gets points
    // Add points to local player score
    ChangePlayerCount(SCORE_KEY, m_points); 
  }
}

int Baddie::GetDamage() const
{
  return m_damage;
}
}


