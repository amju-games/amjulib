#include <LoadVec3.h>
#include <LoadVec2.h>
#include <GameObjectFactory.h>
#include "Harmless.h"
#include "GameConsts.h"
#include "SpriteNode.h"

namespace Amju
{
GameObject* CreateHarmless()
{
  return new Harmless;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Harmless::TYPENAME, CreateHarmless);

const char* Harmless::TYPENAME = "harmless";

const char* Harmless::GetTypeName() const
{
  return TYPENAME;
}

bool Harmless::Load(File* f)
{
  static int id = HARMLESS_START_ID;
  SetId(id++);

  std::string tex;
  if (!f->GetDataLine(&tex))
  {
    f->ReportError("Expected texture filename");
    return false;
  }

  Vec3f pos;
  if (!LoadVec3(f, &pos))
  {
    f->ReportError("Expected position");
    return false;
  }
  SetPos(pos);

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

  SpriteNode* sn = new SpriteNode(tex, cells.x, cells.y, m_size.x * 0.5f, m_size.y * 0.5f);

  SetSceneNode(sn);
  sn->GetSprite()->SetCellRange(0, cells.x * cells.y);
  sn->GetSprite()->SetCell(0);

  return true;
}

void Harmless::Update()
{
  Ve1Object::Update();

  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
  
    if (m_sceneNode->GetAABB())
    {
      static const float YSIZE = 10.0f; // doesn't really matter!
      m_sceneNode->SetAABB(AABB(
        m_pos.x - m_size.x, m_pos.x + m_size.x, // why not / 2 ??
        m_pos.y,            m_pos.y + YSIZE,
        m_pos.z - m_size.y, m_pos.z + m_size.y));
    }
  }
}
}
