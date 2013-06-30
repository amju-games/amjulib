#ifndef BADDIE_NODE_H_INCLUDED
#define BADDIE_NODE_H_INCLUDED

#include <SceneNode.h>
#include "Sprite.h"

namespace Amju
{
class Baddie;

class BaddieNode : public SceneNode
{
public:
  BaddieNode(Baddie*, const std::string tex, 
  int cellsX, int cellsY, float sizeX, float sizeY);

  virtual void Draw();
  virtual void Update();

private:
  Sprite m_sprite;
  Baddie* m_baddie;
};

}

#endif
