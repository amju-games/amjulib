#include "SceneCollisionMesh.h"

#ifdef _DEBUG
//#define SHOW_COLLISION_MESH
#endif

namespace Amju
{
void SceneCollisionMesh::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  SceneMesh::Draw();
  AmjuGL::PopAttrib();

#ifdef SHOW_COLLISION_MESH
  if (m_collMesh)
  {
    m_collMesh->Draw();
  }
#endif // SHOW_COLLISION_MESH
}

}
