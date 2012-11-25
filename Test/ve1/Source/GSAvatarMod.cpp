#include <Timer.h>
#include <AmjuGL.h>
#include <File.h>
#include <ReportError.h>
#include <StringUtils.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSAvatarMod.h"
#include "GSStartGame.h"
#include "Ve1SceneGraph.h"
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
  m_howManyCharsAvailable = 4;
  m_current = 0;
}

void GSAvatarMod::CheckAvailable()
{
  SceneNode* node = GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
 
  if (m_current >= m_howManyCharsAvailable)
  {
    if (node)
    {
      node->SetColour(Colour(0.5f, 0.5f, 0.5f, 0.5f));
      ok->SetIsEnabled(false);
    }
  }
  else
  {
    if (node)
    {
      node->SetColour(Colour(1, 1, 1, 1));
      ok->SetIsEnabled(true);
    }
  }
}

void GSAvatarMod::OnNextType()
{
  AvatarManager* am = TheAvatarManager::Instance();
  m_current++;
  if (m_current == am->GetNumNames())
  {
    m_current = 0;
  }

  m_currentChar = am->GetName(m_current);
  CreateChar();
  CheckAvailable();
}

void GSAvatarMod::OnPrevType()
{
  AvatarManager* am = TheAvatarManager::Instance();
  m_current--;
  if (m_current < 0)
  {
    m_current = am->GetNumNames() - 1;
  }

  m_currentChar = am->GetName(m_current);
  CreateChar();
  CheckAvailable();
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

  static const float Y = ROConfig()->GetFloat("avatar-cam-y", 60.0f);
  AmjuGL::LookAt(0, Y, 120,    0, 0, 0,    0, 1, 0);

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
    m_currentChar = "marge";
  }
  if (!TheAvatarManager::Instance()->GetIndex(m_currentChar, &m_current))
  {
    std::cout << "Couldn't find name in Avatar Manager: " << m_currentChar << "\n";
    Assert(0);
  }
  CreateChar();
  CheckAvailable();
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
