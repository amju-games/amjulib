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
  if (m_model)
  {
    m_model->DrawModel();
  }  
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
  m_model->SetTorsoAnimation("BOTH_DEATH1");
  m_model->SetLegsAnimation("LEGS_RUN");
}

}

