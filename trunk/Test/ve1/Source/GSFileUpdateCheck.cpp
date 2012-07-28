#include "GSFileUpdateCheck.h"
#include <Game.h>
#include <AmjuGL.h>
#include <ConfigFile.h>
#include "GSWaitForNewLocation.h"
#include "FileUpdater.h"
#include "GSStartGame.h"
#include "SaveConfig.h"

namespace Amju
{
static const char* FILE_UPDATE_TIMESTAMP = "file_update_time";

void OnSkip()
{
  TheGSFileUpdateCheck::Instance()->OnSkip();
}

GSFileUpdateCheck::GSFileUpdateCheck()
{
  m_timestamp = TheGameConfigFile::Instance()->GetValue(FILE_UPDATE_TIMESTAMP);
  if (m_timestamp == "")
  {
    m_timestamp = "1";
  }
//  m_next = TheGSWaitForNewLocation::Instance();
}

void GSFileUpdateCheck::Update()
{
  GSGui::Update();
}

void GSFileUpdateCheck::OnFinishedChecking(const std::string& timestamp)
{
  m_timestamp = timestamp;
  TheGameConfigFile::Instance()->Set(FILE_UPDATE_TIMESTAMP, m_timestamp);
  SaveConfig();

  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

void GSFileUpdateCheck::OnSkip()
{
  // TODO Hmm, could be complicated as request is already in flight..??
  // Ignore response if this state is no longer current.

  // DON'T update timestamp
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

void GSFileUpdateCheck::Draw()
{
  GSGui::Draw();

}

void GSFileUpdateCheck::Draw2d()
{
  GSGui::Draw2d();
}

void GSFileUpdateCheck::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-fileupdatecheck.txt");
  Assert(m_gui);

  m_gui->GetElementByName("skip-button")->SetCommand(Amju::OnSkip);

  // Create request
  // Url contains timestamp of last check
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPDATE_CHECK);
  url += "&time=" + m_timestamp;
  TheVe1ReqManager::Instance()->AddReq(new FileUpdater(url));
}

bool GSFileUpdateCheck::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSFileUpdateCheck::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
