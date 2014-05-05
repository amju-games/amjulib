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
  std::string namestr = player->GetName();

  if (player->IsLocalPlayer())
  {
    namestr += " (you!)";
    name->SetFgCol(Colour(0, 0.5f, 0, 1));
  }
  /*
  // This has never been reliable
  else if (player->IsLoggedIn())
  {
    namestr += " - online now!";
    name->SetFgCol(Colour(1, 0, 0, 1));
  }
  */
  else
  {
    name->SetFgCol(Colour(0, 0, 0, 1));
  }
  name->SetText(namestr);

  // Set food, health, treasure scores for this player
  GuiText* t = (GuiText*)GetElementByName(gui, "health-num");
  Assert(t);
  if (player->Exists(HEALTH_KEY))
  {
    t->SetText(player->GetVal(HEALTH_KEY));
  }
  else
  {
    t->SetText("0");
  }

  t = (GuiText*)GetElementByName(gui, "food-num");
  Assert(t);
  if (player->Exists(FOOD_STORED_KEY))
  {
    t->SetText(player->GetVal(FOOD_STORED_KEY));
  }
  else
  {
    t->SetText("0");
  }

  t = (GuiText*)GetElementByName(gui, "treasure-num");
  Assert(t);
  if (player->Exists(TREASURE_KEY))
  {
    t->SetText(player->GetVal(TREASURE_KEY));
  }
  else
  {
    t->SetText("0");
  }

  t = (GuiText*)GetElementByName(gui, "score-text");
  Assert(t);
  if (player->Exists(SCORE_KEY))
  {
    t->SetText("SCORE: " + player->GetVal(SCORE_KEY));
  }
  else
  {
    t->SetText(""); // TODO
  }

#ifdef SHOW_STATS_ON_HOME_PAGE
  // Food given/recvd
  t = (GuiText*)GetElementByName(gui, "more-stats2-text");
  Assert(t);
  std::string str;
  if (player->Exists(FOOD_GIVEN_KEY))
  {
    str = "Food given: " + player->GetVal(FOOD_GIVEN_KEY) + "     ";
  }
  if (player->Exists(FOOD_RECEIVED_KEY))
  {
    str += "Food received: " + player->GetVal(FOOD_RECEIVED_KEY);
  }
  t->SetText(str);

  // Treasure given/recvd
  t = (GuiText*)GetElementByName(gui, "more-stats3-text");
  Assert(t);
  str = "";
  if (player->Exists(TREASURE_GIVEN_KEY))
  {
    str = "Treasure given: " + player->GetVal(TREASURE_GIVEN_KEY) + "     ";
  }
  if (player->Exists(TREASURE_RECEIVED_KEY))
  {
    str += "Treasure received: " + player->GetVal(TREASURE_RECEIVED_KEY);
  }
  t->SetText(str);
#endif 
}
}
