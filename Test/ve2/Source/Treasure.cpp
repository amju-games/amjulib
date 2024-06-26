#include <GameObjectFactory.h>
#include <LoadVec3.h>
#include <LoadVec2.h>
#include <SoundManager.h>
#include "Treasure.h"
#include "Player.h"
#include "GameConsts.h"
#include "HeartCount.h"
#include "SpriteNode.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"

namespace Amju
{
const char* Treasure::TYPENAME = "treasure";

static const float XSIZE = 20.0f;
static const float YSIZE = 50.0f;

std::string MakeTreasureKey()
{
  return "treasure_" + ToString(GetLocalPlayerLocation());
}

GameObject* CreateTreasure()
{
  return new Treasure;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Treasure::TYPENAME, CreateTreasure);

Treasure::Treasure()
{
  m_points = 0;
}

const char* Treasure::GetTypeName() const
{
  return TYPENAME;
}

void Treasure::OnCollidePlayer(Player* p)
{
  if (m_sceneNode && p->IsLocalPlayer())
  {
    // Hide
    SetHidden(true);

    p->GetEffect()->SetColour(Colour(0.8f, 1, 1, 1));
    p->ShowAttacked();

    // Sound fx - TODO different wavs
    // TODO CONFIG
    TheSoundManager::Instance()->PlayWav("sound/cashreg.wav");
    TheSoundManager::Instance()->PlayWav("sound/bonus_points.wav");

    // Add points to local player score
    ChangePlayerCount(SCORE_KEY, m_points); 

    // Set key in database so this treasure cannot be collected again
    p->SetKeyVal(MakeTreasureKey(), "y"); // immediate effect
    TheObjectUpdater::Instance()->SendUpdateReq(p->GetId(), MakeTreasureKey(), "y");

    ChangePlayerCount(TREASURE_KEY, +1);
  }
}

bool Treasure::Load(File* f)
{
  static int id = TREASURE_START_ID;
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

  if (!f->GetInteger(&m_points))
  {
    f->ReportError("Expected num points");
    return false;
  }

  // Only if player has not collected treasure in the current room
  //  before!
  Player* p = GetLocalPlayer();
  // Create scene node if no local player yet OR player exists and there is no treasure key for this room 
  if (!p || !(p->Exists(MakeTreasureKey())))
  {
    // Size depends on points value
    float size = XSIZE * (float)m_points / 500.0f;
    SpriteNode* sn = new SpriteNode(tex, cells.x, cells.y, size * 0.5f, size * 0.5f);

    SetSceneNode(sn);
    sn->GetSprite()->SetCellRange(0, cells.x * cells.y);
    sn->GetSprite()->SetCell(0);

    m_glow = new SpriteNode("flare.png", 1, 1, size, size);
    sn->AddChild(m_glow.GetPtr());

    File f;
    if (!f.OpenRead("fighteffect.txt"))
    {
      f.ReportError("Failed to load effect");
      Assert(0);
    }
    m_sparkle = new AttackEffect;
    if (!m_sparkle->Load(&f))
    {
      f.ReportError("Failed to load effect");
      Assert(0);
    }
    sn->AddChild(m_sparkle.GetPtr());
  }

  return true;
}

void Treasure::Update()
{
  // TODO Doesn't move, so no need to do this every frame, right?
  Ve1Object::Update();

  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    m_glow->SetLocalTransform(m); // ??? WHY ???
  
    if (m_sceneNode->GetAABB())
    {
      m_sceneNode->SetAABB(AABB(
        m_pos.x - XSIZE, m_pos.x + XSIZE,
        m_pos.y,         m_pos.y + YSIZE,
        m_pos.z - XSIZE, m_pos.z + XSIZE));
    }

    // Delete scene node if we have already been collected
    // (Extra check in case player not created yet when Load() called)
    std::string key = MakeTreasureKey();
    Assert(GetLocalPlayer());
    if (GetLocalPlayer()->Exists(key))
    {
      // Or OnLocationExit ..?
      SetHidden(true);
      m_sceneNode = 0;
    }
  }
  GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y,         m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);
}
}
