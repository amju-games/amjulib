#include <AmjuGL.h>
#include <ReportError.h>
#include "Ve1SpriteNode.h"
#include "LayerGroup.h"

namespace Amju
{
// Size of player sprites
static const float SIZE = 40.0f;

LayerSprite& Ve1SpriteNode::GetSprite() 
{
  return m_sprite;
}

void Ve1SpriteNode::SetLayerTex(int layer, int texIndex)
{
  m_sprite.SetLayerTex(layer, texIndex);
}

void Ve1SpriteNode::SetLayerColour(int layer, int colIndex)
{
  m_sprite.SetLayerColour(layer, colIndex);
}

void Ve1SpriteNode::SetFromCharacterName(const std::string& s)
{
std::cout << "Setting sprite from character name...\n";

  // This loads the base spritesheet, which we don't actually draw???
  // (TODO Change LayerSprite so we don't need it)
  std::string tex = "characters/2d/spritesheet.png";
  if (!m_sprite.Load(tex, 16, 1, SIZE * 0.5f, SIZE))
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
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  for (int i = 0; i < 5; i++)
  {
    // Fof layer i, set tex 0 and colour 0
    m_sprite.AddLayer(SpriteLayer(i, lgm->GetTexture(i, 0), lgm->GetColour(i, 0), 0, 0));  
  }
}

void Ve1SpriteNode::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane

  // Translate so centred on sprite
  static const float HSIZE = SIZE * 0.5f;
  AmjuGL::Translate(-HSIZE, -HSIZE, 0); 

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

