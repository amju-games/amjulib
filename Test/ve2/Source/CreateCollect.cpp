#include <Game.h>
#include "CreateCollect.h"
#include "LocalPlayer.h"
#include "Collect.h"

namespace Amju
{
void CreateCollect(int type, int specialId)
{
  Player* player = GetLocalPlayer();
  if (!player)
  {
    Assert(0);
    return;
  }

  Collect* c = new Collect;
  static int id = 1000000;
  id++;
  c->SetId(id); // Game obj ID

  c->SetSpecialId(specialId); // collect ID to validate on server
 
  // Set location (necessary ?)
  c->SetLocation(GetLocalPlayerLocation());

  // Set position
  c->SetPos(player->GetPos());
  
  // TODO TEMP TEST
  // Set pos first
  static const int NUM = 2;
  static const char* MESH_TEX[NUM][2] = 
  {
    { "heart.obj", "heart2.png" },
    { "bean.obj",  "bean1.png" }
  };
  int r = 0; // TODO    rand() % NUM;
  c->Create(MESH_TEX[r][0], MESH_TEX[r][1]);

  c->OnLocationEntry();

  // Add to Game - these are temp, not cached
  TheGame::Instance()->AddGameObject(c);
}

}

