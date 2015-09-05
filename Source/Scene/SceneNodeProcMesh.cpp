#include "SceneNodeProcMesh.h"

namespace Amju
{
void SceneNodeProcMesh::UseMaterial()
{
//std::cout << "SceneNodeProcMesh: no material\n";

  if (m_material)
  {
/*
std::cout << "SceneNodeProcMesh: using material\n";
if (m_material->m_cubemap)
  std::cout << "  .. which DOES have a cubemap!\n";
*/

    m_material->UseThisMaterial();
  }
}
}

