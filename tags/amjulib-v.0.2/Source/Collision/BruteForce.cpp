#include <AmjuFirst.h>
#include <Game.h>
#include "BruteForce.h"
#include <AmjuFinal.h>

namespace Amju
{
void BruteForce::Update()
{
  static Game* game = TheGame::Instance();
  GameObjects* gameobjs = game->GetGameObjects();

  // Copy so iterator not invalidated
  // TODO This should be a flag
  GameObjects gos(*gameobjs);

  for (GameObjects::iterator it = gos.begin(); it != gos.end(); ++it)
  {
    PGameObject go1 = it->second;

    GameObjects::iterator jt = it;
    ++jt;
    for ( ; jt != gos.end(); ++jt)
    {
      PGameObject go2 = jt->second;

      // The collision test is a pluggable function, so could be dynamic etc.
      if (m_collideFunc(go1, go2))
      {
        TheCollisionManager::Instance()->HandleCollision(go1, go2);
      }
    }
  }
}
}

