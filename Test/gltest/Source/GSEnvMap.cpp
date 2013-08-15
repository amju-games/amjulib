#include <AmjuGL.h>
#include <Teapot.h>
#include <ResourceManager.h>
#include <Texture.h>
#include "GSEnvMap.h"

namespace Amju
{
GSEnvMap::GSEnvMap()
{
  //m_nextState=...
}

void GSEnvMap::Update()
{
//  GSBase::Update();

}

void GSEnvMap::Draw()
{
  AmjuGL::SetClearColour(Colour(1, 0, 0, 1));
  GSBase::Draw();

  static Teapot tp;

  static Texture* tex = 
    (Texture*)TheResourceManager::Instance()->GetRes("spheremap_bar.png");
//  tex->UseThisTexture();

  tp.Draw();
}

void GSEnvMap::Draw2d()
{
}

void GSEnvMap::OnActive()
{
}

bool GSEnvMap::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSEnvMap::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
