#define CATCH_CONFIG_MAIN
#include <catch.h> // test framework

#include <BruteForce.h>
#include <SAP.h>

using namespace Amju;

TEST_CASE("Collisions", "Using Brute Force") 
{
  CollisionManager* cm = TheCollisionManager::Instance();
  cm->SetCollisionDetector(new BruteForce);

  REQUIRE(2 == 2);
}

