#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include "GSWaitForUpload.h"
#include "GSEdit.h"
#include <AmjuFinal.h>

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
  if (m_gui)
  {
    GuiText* text = dynamic_cast<GuiText*>(GetElementByName(m_gui, "error"));
    Assert(text);
    text->SetText(str);
  }
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
  }
  else
  {
    SetError("Uploaded " + ToString(m_uploadedFiles) + " of " + ToString(m_totalFiles) + "  files...");
  }
}

} // namespace
