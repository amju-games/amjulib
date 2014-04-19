#include <AmjuFirst.h>
#include <iostream>
#include "GSNetError.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSTitle.h"
#include <GuiText.h>
#include <GuiButton.h>
#include "GSYesNoQuitProcess.h"
#include <AmjuFinal.h>

namespace Amju
{
void ShowError(const std::string& errorStr)
{
  static GSNetError* gs = TheGSNetError::Instance();
  gs->SetError(errorStr);
  TheGame::Instance()->SetCurrentState(gs);
}

void ShowError(const std::string& errorStr, GameState* prevState)
{
  static GSNetError* gs = TheGSNetError::Instance();
  gs->SetError(errorStr);
  gs->SetPrevState(prevState);
  TheGame::Instance()->SetCurrentState(gs);
}

void OnErrorOk()
{
  TheGSNetError::Instance()->GoBack();
}

void OnErrorQuit()
{
  static GSYesNoQuitProcess* gs = TheGSYesNoQuitProcess::Instance();
  gs->SetPrevState(TheGSNetError::Instance()->GetPrevState());
  TheGame::Instance()->SetCurrentState(gs); 
}

GSNetError::GSNetError()
{
  m_prevState = 0;
}

void GSNetError::SetError(const std::string& error)
{ 
  m_errorStr = error;

std::cout << "ERROR: " << error << "\n";
}

void GSNetError::Update()
{
  GSGui::Update();

}

void GSNetError::Draw()
{
  GSGui::Draw();

}

void GSNetError::Draw2d()
{
  GSGui::Draw2d();
}

void GSNetError::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-error.txt");
  Assert(m_gui);

////  m_gui->GetElementByName("error-ok-button")->SetCommand(Amju::OnErrorOk);
////  m_gui->GetElementByName("error-quit-button")->SetCommand(Amju::OnErrorQuit);
  
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "error-ok-button");
  ok->SetCommand(Amju::OnErrorOk);
  ok->SetHasFocus(true);

  GuiButton* quit = (GuiButton*)GetElementByName(m_gui, "error-quit-button");
  quit->SetCommand(Amju::OnErrorQuit);
  quit->SetIsCancelButton(true);


  GuiText* t = dynamic_cast<GuiText*>(m_gui->GetElementByName("error"));
  Assert(t);
  t->SetText(m_errorStr);
}

bool GSNetError::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSNetError::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
