#pragma once

#include "Ve1SpriteNode.h"

namespace Amju
{
class Player;
class GuiElement;

// Mixin class for GUI-based states for VE3
class Ve3ShowPlayer
{
public:
  // Set sprite, GUI stats, etc.
  void ShowPlayer(Player* player, GuiElement* gui);

protected:
  Ve1SpriteNode m_spriteNode;

};
}
