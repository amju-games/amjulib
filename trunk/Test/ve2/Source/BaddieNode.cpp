#include <AmjuGL.h>
#include <ReportError.h>
#include <AmjuRand.h>
#include "BaddieNode.h"
#include "Baddie.h"

namespace Amju
{
static const float SIZE = 20.0f; // TODO

BaddieNode::BaddieNode(
  Baddie* baddie, const std::string tex, 
  int cellsX, int cellsY, float sizeX, float sizeY) : m_baddie(baddie)
{
  //std::string tex = "food1.png";
  // TODO 2 * 2 cells, depends on sprite layout
  if (!m_sprite.Load(tex, cellsX, cellsY, sizeX, sizeY))
  {
    ReportError("FAILED TO LOAD baddie sprite");
    Assert(0);
  }

  m_sprite.SetCellTime(0.1f);
  int c = 0; // start at cell 0, right ?
  m_sprite.SetCellRange(c, c);
  m_sprite.SetCell(c);

  SetBlended(true);
}

void BaddieNode::Update()
{
  // Visible from player ?
  // TODO

  m_aabb = *(m_baddie->GetAABB());

  m_sprite.Update();
}

void BaddieNode::Draw()
{
  AmjuGL::PushMatrix();

  AmjuGL::MultMatrix(m_local);
  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane
 
  AmjuGL::Translate(-SIZE, -SIZE, 0); 
  m_sprite.Draw(Vec2f(0, 0), 2.0f);

  AmjuGL::PopMatrix();
}
}
