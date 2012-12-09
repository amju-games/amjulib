#include <GameObjectFactory.h>
#include <Game.h>
#include <DegRad.h>
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
#include "CommandPickup.h"
#include "GameLookup.h"
#include "LurkMsg.h"
#include "HeartCount.h"
#include "ROConfig.h"

//#define AABB_DEBUG

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
  m_collMeshRot = m_dir;
  m_handlePickup = false;
}

void Furniture::SetEditMenu(GuiMenu* menu)
{
  menu->AddChild(new GuiMenuItem("Set obj mesh...", new SetObjMeshCommand(GetId())));
  menu->AddChild(new GuiMenuItem("Rotate", new CommandRotate(this)));
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
  // Lots of problems if furniture can fall. Every furniture will need collision meshes for terrain
  //  and other intersecting objects.
//  static const float GRAVITY = ROConfig()->GetFloat("gravity", -50.0f); 
//  m_acc.y = GRAVITY;

  Ve1Object::Update();

  if (!m_sceneNode)
  {
    return; // assert ?
  }

  // WHY doesn't this work ??? 
  //if (m_handlePickup)
  //{
  //  HandlePickup(m_pickupId);
  //}

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
      static const float PICKUP_Y = ROConfig()->GetFloat("pickup-y", 60.0f);
      m_pos = go->GetPos() + Vec3f(0, PICKUP_Y, 0); 
      Player* p = dynamic_cast<Player*>(go); 
      if (p)
      {
        p->SetCarrying(this);
      }
    }

    // TODO recalc AABB for VFC and picking
  }

  TurnToFaceDir(); // ?

  // If we got moved or rotated, transform Collision Mesh and recalc bounding box
  if ((m_pos - m_collMeshPos).SqLen() > 0.1f || fabs(m_collMeshRot - m_dir) > 0.1f)
  { 
    Matrix m;

    SceneCollisionMesh* scm = dynamic_cast<SceneCollisionMesh*>(m_sceneNode.GetPtr());
    if (scm)
    {
      scm->CalcCollisionMesh();
  
      m.RotateY(DegToRad(m_dir));
      GetCollisionMesh()->Transform(m);

      m.Translate(m_pos);
      GetCollisionMesh()->Transform(m);
      GetCollisionMesh()->CalcAABB(&m_aabb);

      *(m_sceneNode->GetAABB()) = m_aabb;

      m_collMeshPos = m_pos;
      m_collMeshRot = m_dir;
    }
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

  // Obj mesh comes last, as mesh file name is appended to data file contents when we upload
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(meshFilename);
  if (!mesh)
  {
    return false;
  }

  Assert(mesh);
  SceneCollisionMesh* sm = new SceneCollisionMesh;
  sm->SetMesh(mesh);
  sm->CalcCollisionMesh();
  sm->GetCollisionMesh()->CalcAABB(&m_aabb);
  *(sm->GetAABB()) = m_aabb;

  SetSceneNode(sm);

#ifdef AABB_DEBUG
std::cout << "Got AABB for " << *this << " size: " << m_aabb.GetXSize() << " " << m_aabb.GetYSize() << " " << m_aabb.GetZSize() << "\n";
#endif

  return true;
}

void Furniture::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);

  if (key == SET_KEY("pickup"))
  {
    int pickupId = ToInt(val);
    m_handlePickup = true;
    // Do this later - too soon causes problems
    HandlePickup(pickupId);
  }
}

void Furniture::HandlePickup(int pickupId)
{
  m_handlePickup = false;

  // pickupId is the new owner

  if (pickupId == 0)
  {
    // DROP    
#ifdef PICKUP_DEBUG
std::cout << "Got drop msg... ";
#endif

    // Set down: change height to player height. Put player on top.
    if (m_pickupId == 0)
    {
#ifdef PICKUP_DEBUG
std::cout << *this << " Got drop msg but no previous owner!\n";
#endif
    }
    else
    {
#ifdef PICKUP_DEBUG
std::cout << *this << " Got drop msg; previous owner ID " << m_pickupId << "\n";
#endif

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

      m_sceneNode->SetColour(Colour(1, 1, 1, 1));
      m_sceneNode->SetBlended(false);
    }
  }
  else
  {
    // Pick up msg

    GameObject* go = TheGame::Instance()->GetGameObject(pickupId);
    if (go)
    {
#ifdef PICKUP_DEBUG
std::cout << *this << " got picked up by " << *go << "\n";
#endif
      if (pickupId == GetLocalPlayerId())
      {
std::cout << "That's me! Local player picked up this object!\n";

        // Show drop button
        TheGSMain::Instance()->ShowDropButton(this, true);

        m_sceneNode->SetColour(Colour(1, 1, 1, 0.5f));
        m_sceneNode->SetBlended(true);
      }

      Player* p = dynamic_cast<Player*>(go); // TODO Always a player ?
      if (p)
      {
        p->SetCarrying(this);     
      }
    }
    else
    {
#ifdef PICKUP_DEBUG
std::cout << *this << " got picked up by object " << pickupId << " but this object not created yet!!\n";
#endif
    }
  }
#ifdef PICKUP_DEBUG
std::cout << "Setting m_pickupId to " << pickupId << "\n";
#endif
  m_pickupId = pickupId;
}

CommandPickUp::CommandPickUp(Ve1Object* f, bool takeNotDrop) : m_f(f), m_takeNotDrop(takeNotDrop) {}

bool CommandPickUp::Do()
{
  // TODO This is special, the server must determine if OK - no other pick up is happening.
  // I.e. it's not just a key/val pair, needs special case code.

  if (m_takeNotDrop)
  {
    int cost = 10; // how many hearts it costs to lift this
    // TODO TEMP TEST -- depends on mass of object

    int hc = 0;
    if (GetHeartCount(&hc))
    {
      if (hc <= cost)
      {
        std::string text = "Too heavy! You need more hearts!";
        LurkMsg lm(GameLookup(text), Colour(1, 1, 1, 1), Colour(1, 0, 0, 1), AMJU_CENTRE); 
        TheLurker::Instance()->Queue(lm);

        return false;
      }
      else
      {
        ChangeHeartCount(-cost); 
      }
    }
  }

  TheObjectUpdater::Instance()->SendUpdateReq(
    m_f->GetId(), SET_KEY("pickup"), (m_takeNotDrop ? ToString(GetLocalPlayerId()) : "0"));
  
  TheGSMain::Instance()->ClearContextMenu();

  return false;
}

CommandRotate::CommandRotate(Furniture* f) : m_f(f) {}

bool CommandRotate::Do()
{
  // TODO Rotation angle:
  // For crates etc, any angle is ok.
  // For fences, AABBs are wrong for non-90 degrees. But we are using collision meshes
  //  which are super accuate.
  
  // OK, using 90 degs so AABBs are not inaccurate for picking
  m_f->SetDir(m_f->GetDir() + 90.0f); 

  TheGSMain::Instance()->ClearContextMenu();

  return false;
}

void Furniture::SetMenu(GuiMenu* menu)
{
  // If not currently carried; if you are strong enough

  if (m_pickupId == 0)
  {
    if (GetLocalPlayer()->GetCarrying())
    {
      std::cout << "can't pick up, already carrying something else!\n";
    }
    else
    {
      AddMenuItem("Pick up", new CommandPickUp(this, true));
    }
  }
  else if (m_pickupId == GetLocalPlayerId())
  {
    AddMenuItem("Put down", new CommandPickUp(this, false));
  }
}

void Furniture::OnPlayerCollision(Player* player)
{
  // We might get pushed away from player, or player could move back -- depends on our "mass".
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

