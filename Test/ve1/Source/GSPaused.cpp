#include "GSPaused.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSMain.h"
#include "GSOptions.h"
#include "GSTitle.h"
#include "GSQuitGame.h"
#include "LocalPlayer.h"
#include "GSCalendar.h"

namespace Amju
{
static void OnCalButton()
{
  TheGSCalendar::Instance()->SetPrevState(TheGSPaused::Instance());
  TheGame::Instance()->SetCurrentState(TheGSCalendar::Instance());
}

static void OnResetButton()
{
  ResetLocalPlayer();
}

static void OnResumeButton()
{
  TheGSPaused::Instance()->GoBack();
}

static void OnOptionsButton()
{
  TheGSOptions::Instance()->SetPrevState(TheGSPaused::Instance());
  TheGame::Instance()->SetCurrentState(TheGSOptions::Instance());
}

static void OnQuitButton()
{
  //TheGSYesNoQuitProcess::Instance()->SetPrevState(TheGSMain::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSYesNoQuitProcess::Instance());

  TheGSQuitGame::Instance()->SetPrevState(TheGSMain::Instance());
  TheGame::Instance()->SetCurrentState(TheGSQuitGame::Instance());
}

GSPaused::GSPaused()
{
}

void GSPaused::Update()
{
  GSGui::Update();

}

void GSPaused::Draw()
{
  GSGui::Draw();

}

void GSPaused::Draw2d()
{
  GSGui::Draw2d();
}

void GSPaused::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-paused.txt");
  Assert(m_gui);

  m_gui->GetElementByName("resume-button")->SetCommand(Amju::OnResumeButton);
  m_gui->GetElementByName("options-button")->SetCommand(Amju::OnOptionsButton);
  m_gui->GetElementByName("quit-button")->SetCommand(Amju::OnQuitButton);
  m_gui->GetElementByName("reset-button")->SetCommand(Amju::OnResetButton);
  m_gui->GetElementByName("calendar-button")->SetCommand(Amju::OnCalButton);
}

bool GSPaused::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSPaused::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
