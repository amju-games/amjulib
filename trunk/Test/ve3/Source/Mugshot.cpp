#include <AmjuGL.h>
#include <GuiRect.h>
#include "Mugshot.h"
#include "LayerGroup.h"
#include "Player.h"

namespace Amju
{
void Mugshot::Draw() 
{
  // TODO Draw bg
  GuiRect rect;
  rect.SetSize(Vec2f(2, 2));
  rect.SetLocalPos(Vec2f(-1, 1));
  rect.SetColour(Colour(0, 1, 1, 1));
  //rect.Draw();

  // Draw player - TODO Centre head in frame
  AmjuGL::PushMatrix();
  AmjuGL::RotateX(90.0f); 
  Vec2f pos = GetCombinedPos();
  
  AmjuGL::Translate(pos.x + 0.08f, 0, -pos.y + 0.15f ); 
  AmjuGL::Scale(0.004f, 1, 0.004f);
  m_spriteNode.Draw();
  AmjuGL::PopMatrix();
}

void Mugshot::Init(Player* player)
{
  Assert(player);
  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(player->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

}
