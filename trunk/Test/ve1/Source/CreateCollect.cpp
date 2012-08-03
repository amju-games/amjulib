#include <Game.h>
#include "CreateCollect.h"
#include "LocalPlayer.h"
#include "Collect.h"

namespace Amju
{
void CreateCollect(int type, int specialId)
{
  Collect* c = new Collect;
  static int id = 1000000;
  id++;
  c->SetId(id);
 
  // .. Load etc

  // Set location (necessary ?)
  c->SetLocation(GetLocalPlayerLocation());

  // Set position
  c->SetPos(GetLocalPlayer()->GetPos());
  
  // TODO TEMP TEST
  // Set pos first
  c->Create("bean.obj", "bean5.png");

  c->OnLocationEntry();

  // Add to Game - these are temp, not cached
  TheGame::Instance()->AddGameObject(c);
}

}

