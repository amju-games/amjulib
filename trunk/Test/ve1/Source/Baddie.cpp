#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include "Baddie.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"
#include "LocalPlayer.h"
#include "AttackEffect.h"
#include "FightReq.h"

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
  // Create Scene Node, but don't attach to SceneGraph until needed
  Ve1Character* node = new Ve1Character(this);

  m_sceneNode = node;

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

class CommandFight : public GuiCommand
{
public:
  CommandFight(Ve1ObjectChar* opp) : m_opponent(opp)
  {
  }

  virtual bool Do()
  {
    // Show visual effect. Send request to server.
    Player* loc = GetLocalPlayer();
    Vec3f pos = m_opponent->GetPos();
    pos.y += 40.0f; // TODO TEMP TEST
    loc->MoveTo(pos);

    Vec3f vel = loc->GetVel();
    vel.y += YSIZE;
    loc->SetVel(vel);

    SendFightReq(m_opponent);

//    m_opponent->ShowAttacked();
    
    return false;
  }

private:
  Ve1ObjectChar* m_opponent;
};

void Baddie::SetMenu(GuiMenu* menu)
{
  menu->AddChild(new GuiMenuItem("FIGHT!!", new CommandFight(this))); 
}

}


