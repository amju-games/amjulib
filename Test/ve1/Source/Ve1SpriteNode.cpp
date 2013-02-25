#include <AmjuGL.h>
#include <ReportError.h>
#include "Ve1SpriteNode.h"

namespace Amju
{
void Ve1SpriteNode::SetFromCharacterName(const std::string& s)
{
std::cout << "Setting sprite from character name...\n";

  std::string tex = "spritesheet.png";
  if (!m_sprite.Load(tex, 16, 1))
  {
    ReportError("FAILED TO LOAD sprite from char name");
    assert(0);
  }

  m_sprite.SetCellTime(0.25f);
  m_sprite.SetCellRange(8, 11);
  m_sprite.SetCell(0);
}

void Ve1SpriteNode::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  m_sprite.Draw(Vec2f(0, 0), 1.0f);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PopMatrix();
}

void Ve1SpriteNode::Update()
{
  m_sprite.Update();
}

void Ve1SpriteNode::SetAnim(Ve1SpriteNode::Anim anim)
{
  // TODO Set anim range here
}
}

