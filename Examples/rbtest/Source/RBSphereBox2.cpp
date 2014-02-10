#include "RBBox2.h"
#include "RBSphere2.h"
#include "RBManager.h"

namespace Amju
{
void RBBoxSphere2Collision(RBBox2* box, RBSphere2* s)
{
  // Find p, closest point on box to sphere centre. If dist to centre less than radius, 
  //  there is a collision.
  Vec2f p = s->GetPos();

}


static bool regRBBoxSphere2Collision = TheRBManager::Instance()->AddRBFunc(
  RBBox2::TYPENAME, RBSphere2::TYPENAME, (RBManager::RBFunc)RBBoxSphere2Collision);
}

