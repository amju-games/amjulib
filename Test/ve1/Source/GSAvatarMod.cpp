#include "GSAvatarMod.h"
#include <AmjuGL.h>
#include <File.h>
#include <ReportError.h>
#include <StringUtils.h>
#include "Ve1SceneGraph.h"

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
  m_currentChar = 0;
}

void GSAvatarMod::OnNextType()
{
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->DelChild(m_chars[m_currentChar].GetPtr());
  m_currentChar++;
  if (m_currentChar >= (int)m_chars.size()) 
  {
    m_currentChar = 0;
  }
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_chars[m_currentChar].GetPtr());
}

void GSAvatarMod::OnPrevType()
{
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->DelChild(m_chars[m_currentChar].GetPtr());
  m_currentChar--;
  if (m_currentChar == -1)
  {  
    m_currentChar = m_chars.size() - 1;
  }
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_chars[m_currentChar].GetPtr());
}

void GSAvatarMod::OnCancel()
{
  // Go to previous state, or main state, or title state ?
}

void GSAvatarMod::OnOk()
{
  // TODO Write file, change local player ..?

  // Go to Main state 
  
}

bool GSAvatarMod::LoadCharList()
{
  // Load character types - NB we want this to be downloadable. 
  // Add a new object with asset list to server, which this client will then download...?

  m_chars.clear();

  File f;
  if (!f.OpenRead("charlist.txt"))
  {
    ReportError("Failed to load character list");
    return false;
  }

  int numChars = 0;
  if (!f.GetInteger(&numChars))
  {
    ReportError("Expected number of characters!");
    return false;
  }

  m_chars.reserve(numChars);
  
  for (int i = 0; i < numChars; i++)
  {
    Ve1Character* ch = new Ve1Character;
    if (!ch->Load(&f))
    {
      f.ReportError("Failed to load character " + ToString(i));
      return false;
    }
    m_chars.push_back(ch);
  }

  return true; 
}

void GSAvatarMod::Update()
{
  GSGui::Update();

  GetGuiSceneGraph()->Update();
}

void GSAvatarMod::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

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

//  GetGuiSceneGraph()->Clear() // ???

  LoadCharList();
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_chars[m_currentChar].GetPtr());
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
