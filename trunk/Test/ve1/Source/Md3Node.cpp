#include <AmjuGL.h>
#include "Md3Node.h"

namespace Amju
{
Md3Node::Md3Node()
{
  m_model = 0;
}

void Md3Node::Update()
{
  
}

void Md3Node::Draw()
{
  Assert(IsVisible());
  if (!m_model)
  {
    return;
  }

  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  // TODO Offset Y so feet are at zero
  AmjuGL::Translate(0, 23.0f, 0);
  m_model->DrawModel();
  AmjuGL::PopMatrix();

#ifdef AABB_DEBUG
  DrawAABB(*GetAABB());
#endif
   
}

void Md3Node::SetFromCharacterName(const std::string& chName)
{
  // TODO TEMP TEST - use resource manager
  m_model = new CModelMD3;

  // Load the 3 body part meshes and their skin, textures and animation config files
  std::string path = "characters/" + chName + "/";

  bool bResult = m_model->LoadModel(path, chName);
  if (!bResult)
  {
    std::cout << "Failed to load model.\n";
  }
}

void Md3Node::SetAnim(const std::string& animName)
{
  Assert(m_model);
  static bool once = true;
  if (once)
  {
    once = false;
    m_model->SetTorsoAnimation("BOTH_DEATH1");
    m_model->SetLegsAnimation("LEGS_RUN");
  }
}

}

