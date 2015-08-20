#include <Game.h>
#include "GSLoadLevel.h"
#include "GSTemple.h"
#include "MySceneGraph.h"

namespace Amju
{
void GSTemple::DrawScene()
{
  if (!m_loadedOk)
    return;

  GetSceneGraph()->GetCamera()->SetFromCamera(m_camera);
  GetSceneGraph()->Draw();
}

void GSTemple::OnActive()
{
  GSBase::OnActive();

  m_loadedOk = false;
  GSLoadLevel* LL = TheGSLoadLevel::Instance();
  if (LL->GetLastLoadedLevel() != m_levelFilename)
  {
    LL->SetLevelFile(m_levelFilename);
    LL->SetPrevState(this);
    TheGame::Instance()->SetCurrentState(LL);
    return;
  }
  m_loadedOk = true;

  // TODO start position game object
  m_camera.m_pos.y = 10;

  // Set up barrel dist post process effect
//  m_barrel.SetDrawFunc(DrawStereo);
//  m_barrel.Init();

}

}

