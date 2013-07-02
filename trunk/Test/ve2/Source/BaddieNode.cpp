#include <AmjuGL.h>
#include <ReportError.h>
#include <AmjuRand.h>
#include "BaddieNode.h"
#include "Baddie.h"

namespace Amju
{
BaddieNode::BaddieNode(
  Baddie* baddie, const std::string tex, int cellsX, int cellsY, float sizeX, float sizeY) : 
  SpriteNode(tex, cellsX, cellsY, sizeX, sizeY), m_baddie(baddie)
{
}

void BaddieNode::Update()
{
  // Visible from player ?
  // TODO

  m_aabb = *(m_baddie->GetAABB());

  SpriteNode::Update();
}
}
