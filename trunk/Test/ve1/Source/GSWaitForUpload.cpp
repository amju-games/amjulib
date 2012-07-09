#include <AmjuGL.h>
#include <Game.h>
#include "GSWaitForUpload.h"
#include "GSEdit.h"

namespace Amju
{
static void OnCancel()
{
  TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
}

GSWaitForUpload::GSWaitForUpload()
{
  m_totalFiles = 0;
  m_uploadedFiles = 0;
  m_onFinishedFunc = 0;
}

void GSWaitForUpload::Update()
{
  GSGui::Update();
}

void GSWaitForUpload::Draw()
{
  GSGui::Draw();
}

void GSWaitForUpload::Draw2d()
{
  GSGui::Draw2d();
}

void GSWaitForUpload::OnActive()
{
  // Set before activating this state
  Assert(m_onFinishedFunc);

  GSGui::OnActive();

  m_gui = LoadGui("gui-waitforupload.txt");
  Assert(m_gui);
  GetElementByName(m_gui, "cancel-button")->SetCommand(OnCancel);

  // Total num files must be set by previous state
  SetError("Uploading " + ToString(m_totalFiles) + " files, please wait...");
}

void GSWaitForUpload::SetError(const std::string& str)
{
  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("error"));
  Assert(text);
  text->SetText(str);
}

void GSWaitForUpload::OnUploadFinished(const UploadInfo& ui)
{
  if (ui.m_ok)
  {
    // No mutex required, responses are handled in main thread
    m_uploadedFiles++;

std::cout << "SUCCESS for " << ui.m_filename << "\n";
  }
  else
  {
    SetError("Failed on " + ui.m_filename);
std::cout << "FAIL for " << ui.m_filename << "\n";
    return;
  }

std::cout << " ...num files uploaded now: " << m_uploadedFiles << "\n";

  if (m_uploadedFiles == m_totalFiles)
  {
    // Callback
    Assert(m_onFinishedFunc);
    m_onFinishedFunc();

/*
    if (m_mode == AMJU_EDIT)
    {
std::cout << "All uploaded, we are done here!\n";
      OnLocationCreated();
    }
    else
    {
      SetError("Finished uploading, creating new location on server...");

      std::string dir = MakeLocDir(); //"Loc_" + m_locId; // TODO comon func

      // Send req to make new Location game object.
      std::string url = TheVe1ReqManager::Instance()->MakeUrl(CREATE_LOCATION);
      url += "&loc_id=" + m_locId + "&asset_file=" + dir + "/" + m_assetFilename + "&data_file=" + dir + "/" + m_dataFilename;
      url = ToUrlFormat(url);
      TheVe1ReqManager::Instance()->AddReq(new CreateNewLocationReq(url), m_totalFiles);
    }
*/

  }
  else
  {
    SetError("Uploaded " + ToString(m_uploadedFiles) + " of " + ToString(m_totalFiles) + "  files...");
  }
}

} // namespace
