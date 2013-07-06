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

  m_harmful = false;
  for (int i = 0; i < 4; i++)
  {
    m_time[i] = 0;
    m_cellRange[i] = 0;
  }
  m_timeInState = 0;
  m_maxTimeInState = 0;
}

void Baddie::Update()
{
  Player* lp = GetLocalPlayer();
  if (m_harmful && lp)
  {
    Vec3f v = lp->GetPos() - m_pos;
    v.Normalise();
    v *= m_chaseSpeed;
    SetVel(v);
  }
  else
  {
    SetVel(Vec3f()); // Or move more slowly??
  }

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
    float x = m_size.x * 0.4f;
    float y = m_size.y * 0.4f;

    GetAABB()->Set(
      m_pos.x - x, m_pos.x + x,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - y, m_pos.z + y);

    //if (m_effect)
    //{
    //  *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    //}
  }

  float dt = TheTimer::Instance()->GetDt();
  m_timeInState += dt;
  if (m_timeInState > m_maxTimeInState)
  {
    Assert(dynamic_cast<SpriteNode*>(m_sceneNode.GetPtr()));
    Sprite* spr = ((SpriteNode*)m_sceneNode.GetPtr())->GetSprite();

    m_timeInState = 0;
    m_harmful = !m_harmful;
    // New random time, within range for this state
    if (m_harmful) 
    {
      TheSoundManager::Instance()->PlayWav(m_sound[1]);

      m_maxTimeInState = Rnd(m_time[2], m_time[3]);
      spr->SetCellRange(m_cellRange[2], m_cellRange[3]);
      spr->SetCell(m_cellRange[2]);
    }
    else
    {
      TheSoundManager::Instance()->PlayWav(m_sound[0]);

      m_maxTimeInState = Rnd(m_time[0], m_time[1]);
      spr->SetCellRange(m_cellRange[0], m_cellRange[1]);
      spr->SetCell(m_cellRange[1]);
    }
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
  
  if (!LoadVec2(f, &m_size))
  {
    f->ReportError("Expected size Vec2");
    return false;
  }
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
  m_health = m_maxHealth;

  if (!f->GetFloat(&m_chaseSpeed))
  {
    f->ReportError("Expected chase speed");
    return false;
  }

  BaddieNode* bn = new BaddieNode(this, tex, cells.x, cells.y, m_size.x * 0.5f, m_size.y * 0.5f);

  SetSceneNode(bn);

  for (int i = 0; i < 4; i++)
  {
    if (!f->GetFloat(&m_time[i]))
    {
      f->ReportError("Expected time " + ToString(i));
      return false;
    }
  }

  for (int i = 0; i < 4; i++)
  {
    if (!f->GetInteger(&m_cellRange[i]))
    {
      f->ReportError("Expected cell range " + ToString(i));
      return false;
    }
  }

  m_timeInState = 0;
  m_maxTimeInState = 0;

  // Get sound filenames or <none>
  for (int i = 0; i < 2; i++)
  {
    if (!f->GetDataLine(&m_sound[i]))
    {
      f->ReportError("Expected sound " + ToString(i));
      return false;
    }
    if (m_sound[i] == "<none>")
    {
      m_sound[i] = "";
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
  // No menu, but fire at it

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
  }
}

int Baddie::GetDamage() const
{
  return m_harmful ? m_damage : 0;
}
}


