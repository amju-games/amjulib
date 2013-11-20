#include <GuiText.h>
#include <AmjuAssert.h>
#include "Ve3ShowPlayer.h"
#include "Player.h"
#include "LayerGroup.h"
#include "GameConsts.h"

namespace Amju
{
void Ve3ShowPlayer::ShowPlayer(Player* player, GuiElement* gui)
{
  Assert(player);
  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(player->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());

  m_spriteNode.Update();

  GuiText* name = (GuiText*)GetElementByName(gui, "playername-text");
  Assert(name);
  name->SetText(player->GetName());

  // Set food, health, treasure scores for this player
  GuiText* t = (GuiText*)GetElementByName(gui, "health-num");
  Assert(t);
  if (player->Exists(HEALTH_KEY))
  {
    t->SetText(player->GetVal(HEALTH_KEY));
  }
  else
  {
    t->SetText("?");
  }

  t = (GuiText*)GetElementByName(gui, "food-num");
  Assert(t);
  if (player->Exists(FOOD_STORED_KEY))
  {
    t->SetText(player->GetVal(FOOD_STORED_KEY));
  }
  else
  {
    t->SetText("?");
  }

  t = (GuiText*)GetElementByName(gui, "treasure-num");
  Assert(t);
  if (player->Exists(TREASURE_KEY))
  {
    t->SetText(player->GetVal(TREASURE_KEY));
  }
  else
  {
    t->SetText("?");
  }
}


}
