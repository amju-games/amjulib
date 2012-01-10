#include "GSAvatarMod.h"
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
  m_currentChar = 0;
  m_currentTex = 0;
  m_char = 0;
}

void GSAvatarMod::OnNextColour()
{
  m_currentTex++;
  TheAvatarManager::Instance()->SetTexture(m_currentTex, m_char);
}

void GSAvatarMod::OnPrevColour()
{
  m_currentTex--;
  TheAvatarManager::Instance()->SetTexture(m_currentTex, m_char);
}

void GSAvatarMod::OnNextType()
{
  m_currentChar++;
  TheAvatarManager::Instance()->SetAvatar(m_currentChar, m_char);
}

void GSAvatarMod::OnPrevType()
{
  m_currentChar--;
  TheAvatarManager::Instance()->SetAvatar(m_currentChar, m_char);
}

void GSAvatarMod::OnCancel()
{
  // Go to previous state, or main state, or title state ?
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
}

void GSAvatarMod::OnOk()
{
  PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
  pi->PISetBool(PI_KEY("has set up"), true);

  // This may not be necessary except as an optimisation and so we display the correct type next time
  //  we are in this game state...?
  // TODO Not using this info yet
  pi->PISetInt(PI_KEY("type"), m_currentChar);
  pi->PISetInt(PI_KEY("tex"), m_currentTex);

  // Send update req so all clients show correct avatar settings
  int objId = pi->PIGetInt(PI_KEY("player obj id"));
  TheObjectUpdater::Instance()->SendUpdateReq(objId, SET_KEY("type"), ToString(m_currentChar)); // TODO use int, not string
  TheObjectUpdater::Instance()->SendUpdateReq(objId, SET_KEY("tex"), ToString(m_currentTex)); // TODO use int, not string
  // TODO more avatar settings

  // Go to Main state 
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());  
}

void GSAvatarMod::Update()
{
  GSGui::Update();

  GetGuiSceneGraph()->Update();
}

void GSAvatarMod::Draw()
{
//  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 50, 100,  0, 0, 0,  0, 1, 0);

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
  m_gui->GetElementByName("colour-prev-button")->SetCommand(Amju::OnColourPrevButton);
  m_gui->GetElementByName("colour-next-button")->SetCommand(Amju::OnColourNextButton);

  GetGuiSceneGraph()->Clear();
  GetGuiSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

  m_char = new Ve1Character;
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_char); 

  PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
  if (pi->PIGetBool(PI_KEY("has set up")))
  {
    // Has set up, so we can get previous settings
    m_currentChar = pi->PIGetInt(PI_KEY("type")); 
    m_currentTex = pi->PIGetInt(PI_KEY("tex"));
  }

  TheAvatarManager::Instance()->SetAvatar(m_currentChar, m_char);
  TheAvatarManager::Instance()->SetTexture(m_currentTex, m_char);
}

bool GSAvatarMod::OnCursorEvent(const CursorEvent& ce)
{
  GSGui::OnCursorEvent(ce);
  return false;
}

bool GSAvatarMod::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  GSGui::OnMouseButtonEvent(mbe);
  return false;
}
} // namespace
