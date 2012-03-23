#include "GameState.h"
#include "EventPoller.h"

namespace Amju
{
void GameState::OnActive()
{
  TheEventPoller::Instance()->AddListener(this);
}

void GameState::OnDeactive()
{
  TheEventPoller::Instance()->RemoveListener(this);
}
}
