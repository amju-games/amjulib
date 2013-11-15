#include <AmjuFirst.h>
#include <Outliner.h>
#include "SceneCollisionMesh.h"
#include <AmjuFinal.h>

#ifdef _DEBUG
//#define SHOW_COLLISION_MESH
#endif

namespace Amju
{
SceneMesh* theSceneMesh = 0;

void DrawSceneMesh()
{
  theSceneMesh->Draw();
}

void SceneCollisionMesh::Draw()
{
/*
  static PDrawable dr = AmjuGL::Create(Outliner::TYPE_ID);

  theSceneMesh = this;
  dr->SetDrawFunc(DrawSceneMesh);
  dr->Draw();
*/

  SceneMesh::Draw();

#ifdef SHOW_COLLISION_MESH
  if (m_collMesh)
  {
    m_collMesh->Draw();
  }
#endif // SHOW_COLLISION_MESH
}

}
