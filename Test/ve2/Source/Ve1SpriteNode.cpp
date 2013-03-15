#include <AmjuGL.h>
#include <ReportError.h>
#include "Ve1SpriteNode.h"
#include "LayerGroup.h"

namespace Amju
{
// Size of player sprites
static const float SIZE = 40.0f;

Ve1SpriteNode::Ve1SpriteNode()
{
  SetBlended(true);

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
  m_sprite.SetCellRange(1, 1);
  m_sprite.SetCell(1);

  // Add layers
  // TODO TEMP TEST -- really add them when we get state msgs
  // OR this is OK, the layers are set when we get server msgs. 
  // These are the default settings for each layer.
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  int numLayers = lgm->GetNumLayers();
  for (int i = 0; i < numLayers; i++)
  {
    // Fof layer i, set tex 0 and colour 0
    m_sprite.AddLayer(SpriteLayer(i, lgm->GetTexture(i, 0), lgm->GetColour(0), 0, 0));  
  }
}

LayerSprite& Ve1SpriteNode::GetSprite() 
{
  return m_sprite;
}

void Ve1SpriteNode::SetLayerTex(int layer, int texIndex)
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  if (layer >= lgm->GetNumLayers())
  {
    return;
  }

  m_sprite.SetLayerTex(layer, texIndex);
}

void Ve1SpriteNode::SetLayerColour(int layer, int colIndex)
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  if (layer >= lgm->GetNumLayers())
  {
    return;
  }

  m_sprite.SetLayerColour(layer, colIndex);
}

void Ve1SpriteNode::SetFromCharacterName(const std::string& s)
{
}

void Ve1SpriteNode::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane

  // Translate so centred on sprite
  AmjuGL::Translate(-SIZE * 0.5f, -SIZE * 1.0f, 0); 

  m_sprite.DrawLayers(Vec2f(0, 0), 2.0f);

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PopMatrix();
}

void Ve1SpriteNode::Update()
{
  // Updates cell 
  m_sprite.Update();
}

void Ve1SpriteNode::SetAnimRange(int a, int b)
{
  int c = m_sprite.GetCell();
  m_sprite.SetCellRange(a, b);
  if (c < a || c > b)
  {
    m_sprite.SetCell(a);
  }
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
    int c = 1; //m_sprite.GetCell();
    m_sprite.SetCellRange(c, c);
    m_sprite.SetCell(c);
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
        SetAnimRange(12, 15);
      }
      else
      {
        // Right
        SetAnimRange(8, 11);
      }
    }
    else
    {
      // Up or down
      if (dir.z < 0)
      {
        // Up?
        SetAnimRange(4, 7);
      }
      else
      {
        SetAnimRange(0, 3);
      }
    }
  }
}
}

