#include <AmjuFirst.h>
#include <Timer.h>
#include <Game.h>
#include <DegRad.h>
#include <StringUtils.h>
#include "Ve1ObjectChar.h"
#include "Ve1Character.h"
#include "Terrain.h"
#include "AvatarManager.h"
#include "Useful.h"
#include "GSMain.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "GSDeath.h"
#include "GameMode.h"
#include "ROConfig.h"
#include "Ve1SpriteNode.h"
#include <AmjuFinal.h>

namespace Amju
{
static const float SPEED = 80.0f; // TODO CONFIG

static const float BOUNCE_VEL = -50.0f;

//static const char* TYPE_KEY = "type";
//static const char* TEX_KEY = "tex";
//static const char* AVATAR_KEY = "avatar";
static const char* NAME_KEY = "name";
static const char* STAMINA_KEY = "stamina";

//static const char* DEFAULT_AVATAR = "marge";

Ve1ObjectChar::Ve1ObjectChar()
{
  m_isMoving = false;
  m_recalcHeading = false;

  // ?? SetKeyVal(AVATAR_KEY, DEFAULT_AVATAR);

  // Default, should be set by server
  m_stamina = 3; // TODO CONFIG
  sqLenLastTime = 9999999.9f; // Must be big
}

void Ve1ObjectChar::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);

  if (key == NAME_KEY)
  {
    SetName(val);
  }
  else if (key == STAMINA_KEY)
  {
    m_stamina = ToInt(val); 
    if (m_stamina <= 0)
    {
      m_stamina = 0;
    }

      /*
      // Show player death anim ??
      Ve1Character* vc = dynamic_cast<Ve1Character*>(m_sceneNode.GetPtr());
      if (vc)
      {
        vc->SetAnim(Ve1Character::ANIM_DEATH);
        // Special state IF local player, otherwise will just be respawned by new server pos
        if (GetId() == GetLocalPlayerId())
        {
          //TheGame::Instance()->SetCurrentState(TheGSDeath::Instance());
        }
        // TODO Chat msgs?? Send a lurk msg to any chat recipient ?
      }
      */
    
  }
  else if (key.substr(0, 6) == "sprite")
  {
    Assert(m_sceneNode);
    Ve1SpriteNode* node = dynamic_cast<Ve1SpriteNode*>(m_sceneNode.GetPtr());
    Assert(node);
    // "sprite_tex_0", "sprite_colour_0" for layer 0 sprite info, etc.
    std::string sub = key.substr(0, 10);
    std::string layerStr = key.substr(11);
    int layer = ToInt(layerStr);
    if (sub == "sprite_tex")
    {
      node->SetLayerTex(layer, ToInt(val));
    } 
    else if (sub == "sprite_col")
    {
      node->SetLayerColour(layer, ToInt(val));
    }
    else
    {
      Assert(0);
    }
  }
}

void Ve1ObjectChar::SetEditMenu(GuiMenu* menu)
{
//  menu->AddChild(new GuiMenuItem("Set data file...", new SetDataFile(GetId())));
}

void Ve1ObjectChar::Update()
{
  // Not safe to do anything if the Terrain has not been created yet
  if (!TerrainReady())
  {
    return;
  }

  // TODO Not if underwater ?
  // TODO Put in base class, so we can drop things ?
  static const float GRAVITY = ROConfig()->GetFloat("gravity", -50.0f); 
  m_acc.y = GRAVITY;

  Ve1Object::Update();

  if (IsHidden())
  {
    return;
  }

  // Handle wall collisions with terrain and any building
  if (HandleWalls(GetTerrain()->GetCollisionMesh(), m_oldPos, m_pos))
  {
    m_recalcHeading = true;
  }

  HasCollisionMesh* h = dynamic_cast<HasCollisionMesh*>(m_collidingObject);
  if (h)
  {
    if (HandleWalls(h->GetCollisionMesh(), m_oldPos, m_pos))
    {
      m_recalcHeading = true;
    }
  }

  HandleFloor(GetTerrain()->GetCollisionMesh());
  if (h)
  {  
    HandleFloor(h->GetCollisionMesh());
  }

  // Recalc heading if we are not colliding
  if (m_collidingObject)
  {
    m_recalcHeading = true;
  }


  if (true) //m_recalcHeading)
  {
    m_recalcHeading = false;
    if (m_isMoving)
    {
      MoveTo(m_newPos);
    }
  }
  m_collidingObject = 0;

/*
  // Stop moving if we are close enough to the destination
  // TODO This ends up happening every frame, only do it if we are moving
  if (m_isMoving)
  {
    Vec3f dir = GetPos() - m_newPos;
    dir.y = 0; // ignore y coord for now
    if (dir.SqLen() < 1.0f) // TODO CONFIG
    {
      SetVel(Vec3f(0, 0, 0));
      m_newPos = GetPos();
      SetArrowVis(false);
      m_isMoving = false;
    }
  }
  else
  {
    Assert(GetVel().SqLen() == 0);
  }
*/

  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    //m_sceneNode->Update(); // done for whole scene graph elsewhere

    if (m_shadow)
    {
      // Set shadow AABB to same as Scene Node so we don't cull it by mistake
      *(m_shadow->GetAABB()) = *(m_sceneNode->GetAABB());
    }

    if (m_effect)
    {
      *(m_effect->GetAABB()) = *(m_sceneNode->GetAABB());
    }

    static const float XSIZE = ROConfig()->GetFloat("player-aabb-x", 40.0f);
    static const float YSIZE = ROConfig()->GetFloat("player-aabb-y", 10.0f);
    static const float ZSIZE = ROConfig()->GetFloat("player-aabb-z", 40.0f);

    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - ZSIZE, m_pos.z + ZSIZE);

    /*
    NOT FOR 2D
    TurnToFaceDir();
    */

    Ve1Character* vc = dynamic_cast<Ve1Character*>(m_sceneNode.GetPtr());
    if (vc) //// && vc->GetMd2())
    {
      Vec3f v = m_vel;
      v.y = 0;
      float speed = v.SqLen();

      // TODO Simplify -- either moving or idle.
      // NB Speeds should be an avatar variable and level up

      static const float MAX_SPEED = 100.0f; // TODO CONFIG
      static const float RUN_SPEED = MAX_SPEED * 0.5f;
      static const float WALK_SPEED = RUN_SPEED * 0.5f;

      if (speed > RUN_SPEED)
      {
        vc->SetAnim(Ve1Character::ANIM_RUN);
      }
      else if (speed > WALK_SPEED)
      {
        vc->SetAnim(Ve1Character::ANIM_WALK);
      }
      else
      {
        vc->SetAnim(Ve1Character::ANIM_IDLE);
      }
    }
  }
}

const std::string& Ve1ObjectChar::GetName() const
{
  return m_name;
}

void Ve1ObjectChar::SetName(const std::string& name)
{
  m_name = name;
}

bool Ve1ObjectChar::IsMoving() const
{
  return m_isMoving;
}

const Vec3f& Ve1ObjectChar::GetTargetPos()
{
  return m_newPos;
}

void Ve1ObjectChar::MoveTo(const Vec3f& newpos)
{
  m_newPos = newpos;
  m_isMoving = true;

  Vec3f dir = GetPos() - newpos;
  float sqLen = dir.SqLen();
  // TODO Check if distance is greater than last time - if so, we have missed the target!

  static const float STOP_DIST = ROConfig()->GetFloat("stop-dist", 10.0f);
  if (sqLen < STOP_DIST)
  {
    SetVel(Vec3f(0, 0, 0));
    m_isMoving = false; // Not sure why this wasn't here
  }
  // TODO enable this when we are reliably resetting sqLenLastTime, otherwise sometimes characters won't move
  /*
  else if (sqLen > sqLenLastTime)
  {
    SetVel(Vec3f(0, 0, 0));
    m_isMoving = false; // Not sure why this wasn't here
  }
  */
  else
  {
    dir.Normalise();
    SetVel(-dir * SPEED);

    // Work out direction to face
    SetDir(RadToDeg(atan2((double)m_vel.x, (double)m_vel.z)));
  }
  sqLenLastTime = sqLen;
}

AABB* Ve1ObjectChar::GetAABB()
{
  if (!m_sceneNode)
  {
    return 0;
  }

  Assert(m_sceneNode);
  Assert(m_sceneNode->GetAABB());

  return m_sceneNode->GetAABB();
}

void Ve1ObjectChar::ShowAttacked()
{
  m_effect->Start();
}

}

