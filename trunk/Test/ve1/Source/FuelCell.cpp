#include <Game.h>
#include <GameObjectFactory.h>
#include <SceneNode.h>
#include <Billboard.h>
#include <DrawAABB.h>
#include <AmjuRand.h>
#include "FuelCell.h"
#include "LocalPlayer.h"
#include "GameMode.h"
#include "Ve1Node.h"
#include "Ve1SceneGraph.h"
#include "ROConfig.h"

namespace Amju
{
const char* FuelCell::TYPENAME = "fuelcell";

class FuelNode : public SceneNode
{
public:
  virtual void Draw();
  virtual void Update();
};

void FuelNode::Update()
{
  // Visible from player ?
  // TODO
}

void FuelNode::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  // TODO different colour varieties

  DrawSolidAABB(*(GetAABB()));
  // TODO glow

  PopColour();

  AmjuGL::PopAttrib();
}

const char* FuelCellManager::TYPENAME = "fuelcellmanager";

const char* FuelCellManager::GetTypeName() const
{
  return TYPENAME;
}

void FuelCellManager::OnLocationEntry()
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

  // Create fuel cells
  static int id = 20000;
  for (int i = 0; i < 10; i++)
  {
    FuelCell* f = new FuelCell;
    f->SetId(id++);

    float s = ROConfig()->GetFloat("fuel-cell-spread", 200.0f); 
    Vec3f r(Rnd(-s, s), Rnd(0, s), Rnd(-s, s));
    Vec3f p = GetPos() + r; 
    f->SetPos(p);
    f->Load(0);
    TheGame::Instance()->AddGameObject(f);
    f->OnLocationEntry();
  }
}

static const float XSIZE = 20.0f; // TODO CONFIG
static const float YSIZE = 40.0f;

void FuelCellManager::Update()
{
  Ve1Object::Update();

  m_aabb.Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);

  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    *(m_sceneNode->GetAABB()) = m_aabb;
  }
}

GameObject* CreateFuelCellManager()
{
  return new FuelCellManager;
}

GameObject* CreateFuelCell()
{
  return new FuelCell;
}

static bool registered[] = 
{ 
  TheGameObjectFactory::Instance()->Add(FuelCell::TYPENAME, CreateFuelCell),
  TheGameObjectFactory::Instance()->Add(FuelCellManager::TYPENAME, CreateFuelCellManager)
};

const char* FuelCell::GetTypeName() const
{
  return TYPENAME;
}

FuelCell::FuelCell()
{
  m_isPickable = true;
}

bool FuelCell::Load(File*)
{
  Vec3f p = GetPos();
  float s = 5.0f; // TODO CONFIG

  m_aabb.Set(p.x - s, p.x + s, p.y - s, p.y + s, p.z - s, p.z + s); 
  FuelNode* sm = new FuelNode;
  sm->SetAABB(m_aabb);

  SetSceneNode(sm);

  Billboard* bb = new Billboard;
  Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes("flare.png");
  bb->SetTexture(tex);
  bb->SetSize(30.0f); // TODO CONFIG
  bb->SetAABB(m_aabb);

  sm->AddChild(bb);

  return true;
}


} // namespace
