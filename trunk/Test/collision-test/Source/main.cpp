// Usse CATCH to run unit tests to check that Sweep and Prune gives the
//  same results as Brute Force collision system.

#define CATCH_CONFIG_MAIN
#include <catch.h> // test framework

#include <Game.h>
#include <BruteForce.h>
#include <SAP.h>
 
using namespace Amju;

static const float BOXSIZE = 1.0f;
static int g_collisions = 0; // count of collisions

void HandleCollision(GameObject* go1, GameObject* go2)
{
  std::cout << "Collision between objects " << go1->GetId() << " and " 
    << go2->GetId() << "\n";

  g_collisions++;
}

class TestGO : public GameObject
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update() override
  {
    GameObject::Update();
    m_aabb.Set(
      m_pos.x - BOXSIZE, m_pos.x + BOXSIZE,
      m_pos.y - BOXSIZE, m_pos.y + BOXSIZE,
      m_pos.z - BOXSIZE, m_pos.z + BOXSIZE
    );
  }
  virtual AABB* GetAABB() override { return &m_aabb; }

private:
  AABB m_aabb;
};
const char* TestGO::TYPENAME = "testgo";

// Register collision handler
static bool reg = TheCollisionManager::Instance()->Add(
  TestGO::TYPENAME, TestGO::TYPENAME, HandleCollision);

void CollTests()
{
  Game* game = TheGame::Instance();

  const int NUM_GOS = 3;
  GameObject* gos[NUM_GOS];
  for (int i = 0; i < NUM_GOS; i++)
  {
    TestGO* tgo = new TestGO;
    gos[i] = tgo;
    tgo->SetId(i);

    // Set start x positions of boxes so initially not intersecting.
    // TODO Do this for combinations of axes
    tgo->SetPos(Vec3f(i * BOXSIZE * 4.0f, 0, 0));  

    game->AddGameObject(tgo);    
  }

  CollisionManager* cm = TheCollisionManager::Instance();
  g_collisions = 0;

  // Boxes should not be intersecting
  REQUIRE(!gos[0]->GetAABB()->Intersects(*(gos[1]->GetAABB())));

  game->UpdateGameObjects();
  cm->Update();
  REQUIRE(g_collisions == 0);

  // Set so overlapping
  gos[1]->SetPos(Vec3f(0.5f * BOXSIZE, 0, 0));
  game->UpdateGameObjects(); // Recalc bounding boxes
  // Now boxes intersect
  REQUIRE(gos[0]->GetAABB()->Intersects(*(gos[1]->GetAABB())));

  // Updating collision manager should spot the collision
  cm->Update();
  REQUIRE(g_collisions == 1);

  // Second collision should be ignored
  cm->Update();
  REQUIRE(g_collisions == 1);

  // Change pos so still intersecting
  gos[1]->SetPos(Vec3f(0.5f * BOXSIZE, 0.5f * BOXSIZE, 0));
  game->UpdateGameObjects(); // Recalc bounding boxes
  // Now boxes intersect
  REQUIRE(gos[0]->GetAABB()->Intersects(*(gos[1]->GetAABB())));

  // This collision should also be ignored
  cm->Update();
  REQUIRE(g_collisions == 1);

  // Move away so not intersecting
  gos[1]->SetPos(Vec3f(0.5f * BOXSIZE, 4.0f * BOXSIZE, 0));
  game->UpdateGameObjects(); // Recalc bounding boxes
  // Now boxes do not intersect
  REQUIRE(!gos[0]->GetAABB()->Intersects(*(gos[1]->GetAABB())));
  // No collision
  cm->Update();
  REQUIRE(g_collisions == 1);

  gos[1]->SetPos(Vec3f(0.5f * BOXSIZE, 0.5f * BOXSIZE, 0));
  game->UpdateGameObjects(); // Recalc bounding boxes
  // Now boxes intersect
  REQUIRE(gos[0]->GetAABB()->Intersects(*(gos[1]->GetAABB())));
  // Now there is a new collision
  cm->Update();
  REQUIRE(g_collisions == 2);

  game->ClearGameObjects();
}

TEST_CASE("Collisions Using Brute Force", "") 
{
  CollisionManager* cm = TheCollisionManager::Instance();
  cm->SetCollisionDetector(new BruteForce);

  CollTests();
}

TEST_CASE("Collisions Using Sweep and Prune", "") 
{
  CollisionManager* cm = TheCollisionManager::Instance();
  cm->SetCollisionDetector(new SweepAndPrune);

  CollTests();
}

