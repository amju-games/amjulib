#include <AmjuGL.h>
#include <DrawAABB.h>
#include "Player.h"
#include "Md3Node.h"

namespace Amju
{
Md3Node::Md3Node()
{
  m_model = 0;
  m_currentAnim = ANIM_NOT_SET_YET;
}

void Md3Node::Update()
{
  Ve1Character::Update();

  if (!IsVisible())
  {
    return;
  }  

  Player* player = dynamic_cast<Player*>(m_obj.GetPtr());
  if (player && !player->IsLoggedIn())
  {
    return;
  }

  m_model->Update();
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
}

void Md3Node::SetFromCharacterName(const std::string& chName)
{
  // TODO TEMP TEST - use resource manager
  m_model = new CModelMD3;

  // Load the 3 body part meshes and their skin, textures and animation config files
  std::string path = "characters/" + chName + "/";

  bool bResult = m_model->LoadModel(path); //, chName);
  if (!bResult)
  {
    std::cout << "Failed to load model.\n";
  }
}

void Md3Node::SetAnim(Md3Node::Anim anim)
{
  Assert(m_model);
  // Only if anim changes
  // MD3 TODO - extract current model state from drawing/static data

  if (anim == m_currentAnim)
  {
    return;
  }
  m_currentAnim = anim;

  switch (anim) 
  {
  case ANIM_IDLE:
    m_model->SetTorsoAnimation("TORSO_STAND");
    m_model->SetLegsAnimation("LEGS_IDLE");
    break;

  case ANIM_WALK:
    m_model->SetTorsoAnimation("TORSO_STAND2"); // TODO ?
    m_model->SetLegsAnimation("LEGS_WALK");
    break;

  case ANIM_RUN:
    m_model->SetTorsoAnimation("TORSO_STAND2"); // TODO ?
    m_model->SetLegsAnimation("LEGS_RUN");
    break;

  default:
    Assert(0);
  }
}

}

