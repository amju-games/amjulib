#include <AmjuFirst.h>
#include "SceneGameObject.h"
#include "AmjuGL.h"
#include <AmjuFinal.h>

namespace Amju
{
SceneGameObject::SceneGameObject(PGameObject pGo) : m_pGo(pGo)
{
}

void SceneGameObject::UpdateBoundingVol()
{
  // TODO Should we have GameObject::GetAABB, or CalcAABB..?
//  Assert(m_pGo);
//  m_aabb = m_pGo->GetAABB();
}

SceneGameObjectOpaque::SceneGameObjectOpaque(PGameObject pGo) :
  SceneGameObject(pGo)
{
}

void SceneGameObjectOpaque::Draw()
{
  Assert(m_pGo);
}

SceneGameObjectBlended::SceneGameObjectBlended(PGameObject pGo) :
  SceneGameObject(pGo)
{
  SetBlended(true);
}

void SceneGameObjectBlended::Draw()
{
  Assert(m_pGo);
}
}
