#include <AmjuFirst.h>
#include "GSFileUpdateCheck.h"
#include <Game.h>
#include <AmjuGL.h>
#include <ConfigFile.h>
#include "GSTitle.h"
#include "GSLogo.h"
#include "FileUpdater.h"
#include "GSStartGame.h"
#include "SaveConfig.h"
#include <AmjuFinal.h>

//#define SHOW_LOGO

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
std::cout << "Timestamp for file updates: " << m_timestamp << "\n";

  m_numFilesToWaitFor = 0;
  m_numFilesDownloaded = 0;
}

void GSFileUpdateCheck::SetNumFilesToWaitFor(int n, const std::string newtimestamp)
{
  m_numFilesToWaitFor = n;
  m_newtimestamp = newtimestamp;
}

void GSFileUpdateCheck::OnDownloadedFile(const std::string& filename)
{
  // If we are no longer the current state, it doesn't matter
  if (TheGame::Instance()->GetState() != this)
  {
    return;
  }

  m_numFilesDownloaded++;
  if (m_numFilesDownloaded >= m_numFilesToWaitFor)
  {
    // Done! Update to new timestamp
    OnFinishedChecking(m_newtimestamp);

    if (m_gui)
    {
      GuiText* t = (GuiText*)GetElementByName(m_gui, "filename");
      t->SetText(filename);
    }
  }
}

void GSFileUpdateCheck::Update()
{
  GSGui::Update();
}

void GSFileUpdateCheck::NextState()
{
#ifdef SHOW_LOGO
    TheGame::Instance()->SetCurrentState(TheGSLogo::Instance()); 
#else
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
#endif
}

void GSFileUpdateCheck::OnFinishedChecking(const std::string& timestamp)
{
  if (!timestamp.empty() && ToInt(timestamp) > 0)
  {
    m_timestamp = timestamp;
    TheGameConfigFile::Instance()->Set(FILE_UPDATE_TIMESTAMP, m_timestamp);
    SaveConfig();
  }
  else
  {
    std::cout << "Bogus timestamp!!!!: " << timestamp << "\n";
  }

  NextState();
}

void GSFileUpdateCheck::OnSkip()
{
  // TODO Hmm, could be complicated as request is already in flight..??
  // Ignore response if this state is no longer current.

  // DON'T update timestamp
  NextState();
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

  GetElementByName(m_gui, "skip-button")->SetCommand(Amju::OnSkip);

  // New June 2013: Don't check for updated files. Just skip to next state.
  //TheGame::Instance()->SetCurrentState(TheGSWaitForNewLocation::Instance());

  // Create request
  // Url contains timestamp of last check
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPDATE_CHECK);
  url += "&time=" + m_timestamp;
  TheVe1ReqManager::Instance()->SerialReqManager::AddReq(new FileUpdater(url), 1, false);
}

} // namespace
