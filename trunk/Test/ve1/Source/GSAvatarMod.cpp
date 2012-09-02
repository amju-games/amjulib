#include "GSAvatarMod.h"
#include <Timer.h>
#include <AmjuGL.h>
#include <File.h>
#include <ReportError.h>
#include <StringUtils.h>
#include "Ve1SceneGraph.h"
#include <Game.h>
#include "GSStartGame.h"
#include "PlayerInfo.h"
#include "ObjectUpdater.h"
#include "GSStartMenu.h"
#include "AvatarManager.h"
#include "Ve1Object.h"
#include "LocalPlayer.h"
#include "ROConfig.h"

namespace Amju
{
void OnOkButton()
{
  TheGSAvatarMod::Instance()->OnOk();
}

void OnCancelButton()
{
  TheGSAvatarMod::Instance()->OnCancel();
}

void OnColourPrevButton()
{
  TheGSAvatarMod::Instance()->OnPrevColour();
}

void OnColourNextButton()
{
  TheGSAvatarMod::Instance()->OnNextColour();
}

void OnTypePrevButton()
{
  TheGSAvatarMod::Instance()->OnPrevType();
}

void OnTypeNextButton()
{
  TheGSAvatarMod::Instance()->OnNextType();
}


GSAvatarMod::GSAvatarMod()
{
  m_char = 0;
}

void GSAvatarMod::OnNextColour()
{
  //m_currentTex++;
  //TheAvatarManager::Instance()->SetTexture(m_currentTex, m_char);
}

void GSAvatarMod::OnPrevColour()
{
  //m_currentTex--;
  //TheAvatarManager::Instance()->SetTexture(m_currentTex, m_char);
}

void GSAvatarMod::OnNextType()
{
  m_currentChar = TheAvatarManager::Instance()->GetNextName(m_currentChar);
  CreateChar();
}

void GSAvatarMod::OnPrevType()
{
  m_currentChar = TheAvatarManager::Instance()->GetPrevName(m_currentChar);
  CreateChar();
}

void GSAvatarMod::OnCancel()
{
  // Go to previous state, or main state, or title state ?
  GoBack();
}

void GSAvatarMod::OnOk()
{
  PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
//  pi->PISetBool(PI_KEY("has set up"), true);

  // This may not be necessary except as an optimisation and so we display the correct type next time
  //  we are in this game state...?
  // TODO Not using this info yet
//  pi->PISetInt(PI_KEY("avatar"), m_currentChar);

  // Send update req so all clients show correct avatar settings
  int objId = pi->PIGetInt(PI_KEY("player obj id"));
  // OR GetLocalPlayer()->GetId() -- which is better ?
  TheObjectUpdater::Instance()->SendUpdateReq(objId, SET_KEY("avatar"), m_currentChar); 
  // TODO more avatar settings

  GoBack(); 
}

void GSAvatarMod::Update()
{
  GSGui::Update();

  // Don't do this, it will update characters twice
  //GetGuiSceneGraph()->Update();

  float dt = TheTimer::Instance()->GetDt();
  static const float ROT_VEL = ROConfig()->GetFloat("avatar-rot-vel", 50.0);
  m_yrot += ROT_VEL * dt;
}

void GSAvatarMod::Draw()
{
  AmjuGL::SetClearColour(Colour(0.5f, 0.5f, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 100, 120,  0, 50, 0,  0, 1, 0);

  AmjuGL::RotateY(m_yrot);

  GetGuiSceneGraph()->Draw();
}

void GSAvatarMod::Draw2d()
{
  GSGui::Draw2d();
}

void GSAvatarMod::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-avatarmod.txt");
  Assert(m_gui);
  
  m_gui->GetElementByName("ok-button")->SetCommand(Amju::OnOkButton);
  m_gui->GetElementByName("cancel-button")->SetCommand(Amju::OnCancelButton);
  m_gui->GetElementByName("type-prev-button")->SetCommand(Amju::OnTypePrevButton);
  m_gui->GetElementByName("type-next-button")->SetCommand(Amju::OnTypeNextButton);
//  m_gui->GetElementByName("colour-prev-button")->SetCommand(Amju::OnColourPrevButton);
//  m_gui->GetElementByName("colour-next-button")->SetCommand(Amju::OnColourNextButton);

  GetGuiSceneGraph()->Clear();
  GetGuiSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

  // TODO Horribly hardcoded here, use constants
  m_currentChar = GetLocalPlayer()->GetVal("avatar");
  if (m_currentChar.empty())
  {
std::cout << "No avatar set for local player ???\n";
    m_currentChar = "bird1";
  }
  CreateChar();
}

void GSAvatarMod::CreateChar()
{
  static AvatarManager* am = TheAvatarManager::Instance();

  m_char = am->Create(m_currentChar);
  m_char->SetAnim(Ve1Character::ANIM_IDLE);
  ClearGuiSceneGraph();
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_char.GetPtr()); 

}

} // namespace
