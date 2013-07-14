#include <GameObjectFactory.h>
#include <LoadVec3.h>
#include <LoadVec2.h>
#include <SoundManager.h>
#include "Treasure.h"
#include "Player.h"
#include "GameConsts.h"
#include "HeartCount.h"
#include "SpriteNode.h"

namespace Amju
{
const char* Treasure::TYPENAME = "treasure";

static const float XSIZE = 40.0f;
static const float YSIZE = 50.0f;

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
  if (p->IsLocalPlayer())
  {
    // Hide
    SetHidden(true);

    // Sound fx - TODO different wavs
    static const std::string TREASURE_WAV = "woohoo.wav"; // TODO CONFIG
    TheSoundManager::Instance()->PlayWav(TREASURE_WAV);

    // Add points to local player score
    ChangePlayerCount(SCORE_KEY, m_points); 
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

  SpriteNode* sn = new SpriteNode(tex, cells.x, cells.y, XSIZE * 0.5f, XSIZE * 0.5f);

  SetSceneNode(sn);
  sn->GetSprite()->SetCellRange(0, cells.x * cells.y);
  sn->GetSprite()->SetCell(0);

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
  
    if (m_sceneNode->GetAABB())
    {
      m_sceneNode->GetAABB()->Set(
        m_pos.x - XSIZE, m_pos.x + XSIZE,
        m_pos.y,         m_pos.y + YSIZE,
        m_pos.z - XSIZE, m_pos.z + XSIZE);
    }
  }
  GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y,         m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);
}
}
