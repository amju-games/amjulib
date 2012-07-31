#include <GameObjectFactory.h>
#include <Game.h>
#include "Furniture.h"
#include "ObjectUpdater.h"
#include "LocalPlayer.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "GameMode.h"
#include "Player.h"
#include "Useful.h"
#include "SceneCollisionMesh.h"
#include "SetObjMeshCommand.h"
#include "GSMain.h"

namespace Amju
{
const char* Furniture::TYPENAME = "furniture";

GameObject* CreateFurniture()
{
  return new Furniture;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Furniture::TYPENAME, CreateFurniture);

Furniture::Furniture()
{
  // Set pos to out of range, so invisible until pos set
  // ?????????

  m_pickupId = 0; // ID of player picking this object up
}

void Furniture::SetEditMenu(GuiMenu* menu)
{
  menu->AddChild(new GuiMenuItem("Set obj mesh...", new SetObjMeshCommand(GetId())));
}

CollisionMesh* Furniture::GetCollisionMesh()
{
  return (dynamic_cast<SceneCollisionMesh*>(m_sceneNode.GetPtr()))->GetCollisionMesh();
}

const char* Furniture::GetTypeName() const
{
  return TYPENAME;
}

void Furniture::Update()
{
  Ve1Object::Update();

  if (!m_sceneNode)
  {
    return; // assert ?
  }

  if (m_pickupId == 0)
  {
    //Matrix m;
    //m.Translate(m_pos);
    //m_sceneNode->SetLocalTransform(m);
  }
  else
  {
    // Get pos of pickup player. Set scene node pos to this.
    GameObject* go = TheGame::Instance()->GetGameObject(m_pickupId);
    if (go)
    {
      m_pos = go->GetPos() + Vec3f(0, 40, 0); // y offset TODO
      Player* p = dynamic_cast<Player*>(go); 
      if (p)
      {
        p->SetCarrying(this);
      }
    }

    // TODO recalc AABB for VFC and picking
  }

  // TODO Only if we have moved!!!
  // TODO
  if ((m_pos - m_collMeshPos).SqLen() > 0.1f)
  { 
    Matrix m;
    m.Translate(m_pos);
    // TODO Also rotation

    m_sceneNode->SetLocalTransform(m);

    // Recalc collision mesh
    Vec3f v = m_pos - m_collMeshPos;
    m.Translate(v);
    GetCollisionMesh()->Transform(m);

    // Don't need to do this -- the AABB is being translated somewhere else - where ?
    ////m_aabb.Translate(v);
    *(m_sceneNode->GetAABB()) = m_aabb;
    m_sceneNode->GetAABB()->Translate(m_pos);

    m_collMeshPos = m_pos;
  }

  // If moving and like a ball, we should rotate
}

bool Furniture::Load(File* f)
{
  // Create Scene Node, but don't attach to SceneGraph until needed

  // Get mesh filename. 
  std::string meshFilename;
  if (!f->GetDataLine(&meshFilename))
  {
    f->ReportError("Expected mesh file name for furniture");
    return false;
  }

  m_shadow = new Shadow;
//  if (!m_shadow->Load(f))
//  {
//    return false;
//  }

  // TODO Only on Location Entry, surely ?!!  Can we just use Key/Vals for all this ?

  // Obj mesh comes last, as mesh file name is appended to data file contents when we upload
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(meshFilename);
  Assert(mesh);
  SceneCollisionMesh* sm = new SceneCollisionMesh;
  sm->SetMesh(mesh);
  sm->CalcCollisionMesh(mesh);
  sm->GetCollisionMesh()->CalcAABB(&m_aabb);
  *(sm->GetAABB()) = m_aabb;
  m_sceneNode = sm;
std::cout << "Got AABB for " << *this << " size: " << m_aabb.GetXSize() << " " << m_aabb.GetYSize() << " " << m_aabb.GetZSize() << "\n";

//  m_sceneNode->AddChild(m_shadow.GetPtr());
  return true;
}

void Furniture::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);

  if (key == SET_KEY("pickup"))
  {
    int pickupId = ToInt(val);
    HandlePickup(pickupId);
  }
}

void Furniture::HandlePickup(int pickupId)
{
  // pickupId is the new owner

  if (pickupId == 0)
  {
    // DROP    
std::cout << "Got drop msg... ";

    // Set down: change height to player height. Put player on top.
    if (m_pickupId == 0)
    {
std::cout << *this << " Got drop msg but no previous owner!\n";
    }
    else
    {
std::cout << *this << " Got drop msg; previous owner ID " << m_pickupId << "\n";

      GameObject* go = TheGame::Instance()->GetGameObject(m_pickupId);

      if (go)
      {
        float y = go->GetPos().y;
        m_pos.y = y;
        float h = m_aabb.GetYSize();
        go->SetPos(go->GetPos() + Vec3f(0, h + 10.0f, 0)); // 10 is TEMP TEST

        Player* p = dynamic_cast<Player*>(go); // TODO Always a player ?
        if (p)
        {
          p->SetCarrying(0);
        }
      }
    }

    // This looks very wrong - the server should set the position of the object when it is put down.
    // At the very least, only the player who moved the item should position it!?
    if (m_pickupId == GetLocalPlayerId())
    {
      TheObjectUpdater::Instance()->SendPosUpdateReq(GetId(), m_pos, m_location);
      // Hide drop button
      TheGSMain::Instance()->ShowDropButton(this, false);
    }
  }
  else
  {
    // Pick up msg

    GameObject* go = TheGame::Instance()->GetGameObject(pickupId);
    if (go)
    {
std::cout << *this << " got picked up by " << *go << "\n";

      if (pickupId == GetLocalPlayerId())
      {
std::cout << "That's me! Local player picked up this object!\n";

        // Show drop button
        TheGSMain::Instance()->ShowDropButton(this, true);
      }

      Player* p = dynamic_cast<Player*>(go); // TODO Always a player ?
      if (p)
      {
        p->SetCarrying(this);     
      }
    }
    else
    {
std::cout << *this << " got picked up by object " << pickupId << " but this object not created yet!!\n";
    }
  }
std::cout << "Setting m_pickupId to " << pickupId << "\n";
  m_pickupId = pickupId;
}

CommandPickUp::CommandPickUp(Furniture* f, bool takeNotDrop) : m_f(f), m_takeNotDrop(takeNotDrop) {}

bool CommandPickUp::Do()
{
std::cout << "Pick up command!\n";
  // TODO This is special, the server must determine if OK - no other pick up is happening.
  // I.e. it's not just a key/val pair, needs special case code.

  TheObjectUpdater::Instance()->SendUpdateReq(
    m_f->GetId(), SET_KEY("pickup"), (m_takeNotDrop ? ToString(GetLocalPlayerId()) : "0"));

  return false;
}

void Furniture::SetMenu(GuiMenu* menu)
{
  // If not currently carried; if you are strong enough

  if (m_pickupId == 0)
  {
    menu->AddChild(new GuiMenuItem("Pick up", new CommandPickUp(this, true)));
  }
  else if (m_pickupId == GetLocalPlayerId())
  {
std::cout << "Set menu: should be Put down\n";
    menu->AddChild(new GuiMenuItem("Put down", new CommandPickUp(this, false)));
  }
}

void Furniture::OnPlayerCollision(Player* player)
{
  // We might get pushed away from player, or player could move back -- depends on our "mass".

//std::cout << "Guhhhh!!! Collide with furniture!! player: " << player->GetId() << " furniture: " << GetId() << "\n";
}

AABB* Furniture::GetAABB()
{
  if (!m_sceneNode) 
  {
    return 0;
  }

  if (m_pickupId != 0)
  {
//std::cout << *this << " is carried, so no AABB\n";

//    return 0;
  }

  return m_sceneNode->GetAABB();
}
}

