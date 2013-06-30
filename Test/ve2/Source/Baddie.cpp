#include <AmjuFirst.h>
#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include <LoadVec3.h>
#include <LoadVec2.h>
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

static const float XSIZE = 20.0f;
static const float YSIZE = 60.0f;

const char* Baddie::TYPENAME = "baddie";

const char* Baddie::GetTypeName() const
{
  return TYPENAME;
}

Baddie::Baddie() : m_damage(0)
{
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
  //menu->AddChild(new GuiMenuItem("FIGHT!!", new CommandFight(this))); 
////  AddMenuItem("FIGHT!!", new CommandFight(this)); 
}

}


