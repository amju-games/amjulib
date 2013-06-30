#include <AmjuFirst.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <SceneNode.h>
#include <Billboard.h>
#include <DrawAABB.h>
#include <AmjuRand.h>
#include <ReportError.h>
#include "Food.h"
#include "LocalPlayer.h"
#include "GameMode.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "ROConfig.h"
#include "Sprite.h"
#include "Terrain.h"
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* Food::TYPENAME = "Food";

// Size of food sprites
static const float SIZE = 20.0f;

class FoodNode : public SceneNode
{
public:
  FoodNode();
  virtual void Draw();
  virtual void Update();

private:
  Sprite m_sprite;
};

FoodNode::FoodNode()
{
  std::string tex = "food1.png";
  // TODO 2 * 2 cells, depends on sprite layout
  if (!m_sprite.Load(tex, 4, 4, SIZE, SIZE))
  {
    ReportError("FAILED TO LOAD food sprite");
    Assert(0);
  }

  m_sprite.SetCellTime(0.1f);
  // random food
  int c = (int)Rnd(0, 10);
  m_sprite.SetCellRange(c, c);
  m_sprite.SetCell(c);

  SetBlended(true);
}

void FoodNode::Update()
{
  // Visible from player ?
  // TODO

  m_sprite.Update();
}

void FoodNode::Draw()
{
  AmjuGL::PushMatrix();

  AmjuGL::MultMatrix(m_local);
  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane
 
  AmjuGL::Translate(-SIZE, -SIZE, 0); 
  m_sprite.Draw(Vec2f(0, 0), 2.0f);

  AmjuGL::PopMatrix();
}

const char* FoodManager::TYPENAME = "Foodmanager";

const char* FoodManager::GetTypeName() const
{
  return TYPENAME;
}

void SetRandomFoodInLocation()
{
  FoodManager fcm;
  fcm.SetPos(Vec3f(0, 0, 0)); // TODO
  fcm.OnLocationEntry();
}

void FoodManager::OnLocationEntry()
{
  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    // Add node to Scene Graph
    m_sceneNode = new Ve1Node(this);
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->AddChild(m_sceneNode);
    *(m_sceneNode->GetAABB()) = m_aabb;
  }
  // else... ????

  // Create Food cells
  static int id = FOOD_START_ID;
  for (int i = 0; i < 10; i++)
  {
    Food* f = new Food;
    f->SetId(id++);

    float s = ROConfig()->GetFloat("Food-cell-spread", 200.0f); 
    Vec3f r(Rnd(-s, s), 0, Rnd(-s, s));
    Vec3f p = GetPos() + r; 
    //if (TerrainReady())
    //{
    //  GetTerrain()->GetCollisionMesh()->GetY(Vec2f(p.x, p.z), &p.y); // TODO TEMP TEST Set all food to y = 0
    //}
    f->SetPos(p);
    f->Load(0);
    TheGame::Instance()->AddGameObject(f);
    f->OnLocationEntry();
  }
}

void FoodManager::Update()
{
  Ve1Object::Update();

  static const float XSIZE = ROConfig()->GetFloat("food-aabb-x", 40.0f);
  static const float YSIZE = ROConfig()->GetFloat("food-aabb-y", 10.0f);
  static const float ZSIZE = ROConfig()->GetFloat("food-aabb-z", 40.0f);

  GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - ZSIZE, m_pos.z + ZSIZE);

  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    *(m_sceneNode->GetAABB()) = m_aabb;
  }
}

GameObject* CreateFoodManager()
{
  return new FoodManager;
}

GameObject* CreateFood()
{
  return new Food;
}

static bool registered[] = 
{ 
  TheGameObjectFactory::Instance()->Add(Food::TYPENAME, CreateFood),
  TheGameObjectFactory::Instance()->Add(FoodManager::TYPENAME, CreateFoodManager)
};

const char* Food::GetTypeName() const
{
  return TYPENAME;
}

Food::Food()
{
  m_isPickable = true;
  m_owner = 0;
}

bool Food::Load(File*)
{
  Vec3f p = GetPos();

  static const float XSIZE = ROConfig()->GetFloat("food-aabb-x", 40.0f);
  static const float YSIZE = ROConfig()->GetFloat("food-aabb-y", 10.0f);
  static const float ZSIZE = ROConfig()->GetFloat("food-aabb-z", 40.0f);

  m_aabb.Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - ZSIZE, m_pos.z + ZSIZE);

  FoodNode* sm = new FoodNode;
  sm->SetAABB(m_aabb);

  SetSceneNode(sm);

  return true;
}

void Food::Update()
{
  Furniture::Update();

  if (m_owner)
  {
    Vec3f p = m_owner->GetPos();
    // Get dragged behind
    Vec3f diff = p - GetPos();
    float sqdist = diff.SqLen();
    const float MAX_DIST = 30.0f; // TODO CONFIG
    if (sqdist > MAX_DIST * MAX_DIST)
    {
      SetVel(diff); // TODO TEMP TEST
    }
    else
    {
      SetVel(Vec3f());
    }

    p = GetPos();

    static const float XSIZE = ROConfig()->GetFloat("food-aabb-x", 40.0f);
    static const float YSIZE = ROConfig()->GetFloat("food-aabb-y", 10.0f);
    static const float ZSIZE = ROConfig()->GetFloat("food-aabb-z", 40.0f);

    m_aabb.Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - ZSIZE, m_pos.z + ZSIZE);

    m_sceneNode->SetAABB(m_aabb);
  }
}

} // namespace
