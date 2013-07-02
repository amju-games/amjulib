#ifndef BADDIE_NODE_H_INCLUDED
#define BADDIE_NODE_H_INCLUDED

#include "SpriteNode.h"

namespace Amju
{
class Baddie;

class BaddieNode : public SpriteNode
{
public:
  BaddieNode(Baddie*, const std::string tex, 
  int cellsX, int cellsY, float sizeX, float sizeY);

  virtual void Update();

private:
  Baddie* m_baddie;
};

}

#endif
