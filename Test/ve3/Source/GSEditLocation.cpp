#include <AmjuFirst.h>
#include "GSEditLocation.h"
#include <Game.h>
#include "GSEdit.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnOKEdit(GuiElement*)
{
  TheGSEditLocation::Instance()->OnOKButton();
}

static void OnCancelButton(GuiElement*)
{
  // Go back to prev state
  TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
}

void GSEditLocation::RequestNewId()
{
  OnNewIdResponse(m_locId);
}

void GSEditLocation::OnActive()
{
  m_locId = ToString(GetLocalPlayerLocation());

  GSGui::OnActive();

  m_gui = LoadGui("gui-newlocation.txt");
  Assert(m_gui);

  m_gui->GetElementByName("ok-button")->SetCommand(OnOKEdit);
  m_gui->GetElementByName("cancel-button")->SetCommand(OnCancelButton);
  // TODO "test" and "commit" buttons

  // Get last path, set path/file browser to it
}

}

