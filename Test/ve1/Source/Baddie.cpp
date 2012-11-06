#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include "Baddie.h"
#include "AvatarManager.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "AttackEffect.h"
#include "CommandFight.h"

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

static const char* AVATAR_KEY = "avatar";

const char* Baddie::GetTypeName() const
{
  return TYPENAME;
}

Baddie::Baddie()
{
  SetKeyVal(AVATAR_KEY, "dino1");
}

void Baddie::Update()
{
  Ve1ObjectChar::Update();

  if (GetAABB())
  {
    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);

    if (m_effect)
    {
      *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    }
  }
}

bool Baddie::Load(File* f)
{
  if (!m_shadow->Load(f))
  {
    return false;
  }

  // TODO TEMP TEST
  // Particle effect when attacked, etc.
  m_effect = new AttackEffect;
  if (!m_effect->Load(f))
  {
    f->ReportError("Failed to load exit effect");
    return false;
  }
  m_effect->SetVisible(true);

  m_sceneNode->AddChild(m_effect.GetPtr());

  return true;
}

void Baddie::SetMenu(GuiMenu* menu)
{
  //menu->AddChild(new GuiMenuItem("FIGHT!!", new CommandFight(this))); 
  AddMenuItem("FIGHT!!", new CommandFight(this)); 
}

}


