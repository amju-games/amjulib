#include "DragBox.h"

namespace Amju
{
static const float CORNER_SIZE = 0.02f;

DragBox::DragBox()
{
  for (int i = 0; i < 4; i++)
  {
    m_corners[i].SetSize(Vec2f(CORNER_SIZE, CORNER_SIZE));
    m_corners[i].SetColour(Colour(0, 1, 1, 1));
  }
}

void DragBox::Draw()
{
  GuiRect::Draw(); 

  const Vec2f& size = GetSize(); 
  const Vec2f& pos = GetLocalPos();
  m_corners[0].SetLocalPos(pos + Vec2f(CORNER_SIZE, CORNER_SIZE));
  m_corners[1].SetLocalPos(pos + Vec2f(size.x, 0));
  m_corners[2].SetLocalPos(pos + Vec2f(0, -size.y));
  m_corners[3].SetLocalPos(pos + Vec2f(size.x, -size.y));

  m_corners[0].SetLocalPos(GetLocalPos());
  for (int i = 0; i < 4; i++)
  {
    m_corners[i].Draw();
  }
}
}

