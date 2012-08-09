#include <iostream>
#include <Timer.h>
#include "CollisionManager.h"
#include "Player.h"

namespace Amju
{
void CollidePlayerPlayer(Player* p1, Player* p2)
{
  if (p1->IsHidden() || p2->IsHidden())
  {
    return;
  }

  if (!p1->IsLoggedIn() || !p2->IsLoggedIn())
  {
    return;
  }

  // Move both players away from each other
  Vec3f v = p1->GetPos() - p2->GetPos();
  // Avoid divide by zero if objects are set to the same location
  if (v.SqLen() > 1.0f)
  {
    v.Normalise();
  }
  else
  {
    v = Vec3f(1, 0, 0);
  }

  float dt = TheTimer::Instance()->GetDt();
  const float VEL = 100.0f;
  v *= (VEL * dt);
  p1->SetPos(p1->GetPos() + v);
  // Adjust heading
  if (p1->IsMoving())
  {
    p1->MoveTo(p1->GetTargetPos());
  }

  p2->SetPos(p2->GetPos() - v);
  if (p2->IsMoving())
  {
    p2->MoveTo(p2->GetTargetPos());
  }

std::cout << "Whoa, player " << p1->GetId() << " hit player " << p2->GetId() << "!!!\n";
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Player::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerPlayer);
} 
