#include <ReportError.h>
#include "SpriteNode.h"

namespace Amju
{
SpriteNode::SpriteNode(const std::string tex, int cellsX, int cellsY, float sizeX, float sizeY)
{
  if (!m_sprite.Load(tex, cellsX, cellsY, sizeX, sizeY))
  {
    ReportError("FAILED TO LOAD sprite");
    Assert(0);
  }

  m_sprite.SetCellTime(0.1f); // Default - change in subclass 
  int c = 0; // start at cell 0
  m_sprite.SetCellRange(c, c);
  m_sprite.SetCell(c);

  SetBlended(true);
}

void SpriteNode::Update()
{
  m_sprite.Update();
}

void SpriteNode::Draw()
{
  AmjuGL::PushMatrix();

  AmjuGL::MultMatrix(m_local);
  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane
 
  const Vec2f& size = m_sprite.GetSize();

  AmjuGL::Translate(-size.x, -size.y, 0); 
  m_sprite.Draw(Vec2f(0, 0), 2.0f);

  AmjuGL::PopMatrix();
}
}
