#ifndef SPRITE_NODE_H_INCLUDED
#define SPRITE_NODE_H_INCLUDED

#include <SceneNode.h>
#include "Sprite.h"

namespace Amju
{
// Basic Scene Node showing a Sprite. Base class for baddies etc.
class SpriteNode : public SceneNode
{
public:
  SpriteNode(const std::string tex, int cellsX, int cellsY, float sizeX, float sizeY);

  virtual void Draw();

  // Subclasses update AABB then call this
  virtual void Update();

  Sprite* GetSprite() { return &m_sprite; }

private:
  Sprite m_sprite;
};
}

#endif
