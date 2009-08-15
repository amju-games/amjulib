#include "SceneGameObject.h"
#include "AmjuGL.h"

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

//  AmjuGL::PushMatrix();
//  AmjuGL::MultMatrix(m_local);
  m_pGo->Draw();
//  AmjuGL::PopMatrix();
}

SceneGameObjectBlended::SceneGameObjectBlended(PGameObject pGo) :
  SceneGameObject(pGo)
{
  SetBlended(true);
}

void SceneGameObjectBlended::Draw()
{
  Assert(m_pGo);

//  AmjuGL::PushMatrix();
//  AmjuGL::MultMatrix(m_local);
  m_pGo->DrawBlended();
//  AmjuGL::PopMatrix();
}
}
