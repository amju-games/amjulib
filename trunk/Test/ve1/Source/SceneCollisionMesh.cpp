#include "SceneCollisionMesh.h"

#ifdef _DEBUG
//#define SHOW_COLLISION_MESH
#endif

namespace Amju
{
void SceneCollisionMesh::Draw()
{
  SceneMesh::Draw();

#ifdef SHOW_COLLISION_MESH
  if (m_collMesh)
  {
    m_collMesh->Draw();
  }
#endif // SHOW_COLLISION_MESH
}

}
