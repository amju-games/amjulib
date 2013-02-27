#include <AmjuGL.h>
#include <ReportError.h>
#include "Ve1SpriteNode.h"

namespace Amju
{
void Ve1SpriteNode::SetLayerTex(int layer, const std::string& tex)
{
  Texture* t = (Texture*)TheResourceManager::Instance()->GetRes(tex);
  Assert(t);
  m_sprite.SetLayerTex(layer, t);
}

void Ve1SpriteNode::SetLayerColour(int layer, const Colour& col)
{
  m_sprite.SetLayerColour(layer, col);
}

void Ve1SpriteNode::SetFromCharacterName(const std::string& s)
{
std::cout << "Setting sprite from character name...\n";

  // This loads the base spritesheet, which we don't actually draw???
  // (TODO Change LayerSprite so we don't need it)
  std::string tex = "characters/2d/spritesheet.png";
  if (!m_sprite.Load(tex, 16, 1))
  {
    ReportError("FAILED TO LOAD sprite from char name");
    Assert(0);
  }

  m_sprite.SetCellTime(0.25f);
  // TODO TEMP TEST
  m_sprite.SetCellRange(8, 11);
  m_sprite.SetCell(0);

  // Add layers
  // TODO TEMP TEST -- really add them when we get state msgs
  Texture* headTex = (Texture*)
    TheResourceManager::Instance()->GetRes("characters/2d/spritesheet.png");
  m_sprite.AddLayer(SpriteLayer(0, headTex, Colour(1, 1, 1, 1)));  
}

void Ve1SpriteNode::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  m_sprite.DrawLayers(Vec2f(0, 0), 2.0f);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PopMatrix();
}

void Ve1SpriteNode::Update()
{
  // Updates cell 
  m_sprite.Update();
}

void Ve1SpriteNode::SetAnim(Ve1SpriteNode::Anim anim)
{
  // ?
/*
  if (anim == m_currentAnim)
  {
    return;
  }
*/

  // TODO Set anim range here
  // BUT also based on direction
  if (anim == ANIM_IDLE)
  {
    int c = m_sprite.GetCell();
    m_sprite.SetCellRange(c, c);
  }
  else
  {
    Vec3f dir = m_obj->GetVel();
    if (fabs(dir.x) > fabs(dir.z))
    {
      // Left or right
      if (dir.x < 0)
      {
        // Left
        m_sprite.SetCellRange(12, 15);
      }
      else
      {
        // Right
        m_sprite.SetCellRange(8, 11);
      }
    }
    else
    {
      // Up or down
      if (dir.z < 0)
      {
        // Up?
        m_sprite.SetCellRange(4, 7);
      }
      else
      {
        m_sprite.SetCellRange(0, 3);
      }
    }
  }
}
}

