#pragma once

#include <GuiElement.h>
#include "Ve1SpriteNode.h"

namespace Amju
{
class Player;

// Picture of player avatar head for guestbook etc
class Mugshot : public GuiElement
{
public:
  virtual void Draw() override;
  void Init(Player*);

private:
  Ve1SpriteNode m_spriteNode;
};
}
