#include "GSStartMenu.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSLogin.h"
#include "GSStartGame.h"

namespace Amju
{
void OnSingleButton()
{
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

void OnMultiButton()
{
  TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
}

GSStartMenu::GSStartMenu()
{
}

void GSStartMenu::Update()
{
  GSGui::Update();
}

void GSStartMenu::Draw()
{
  GSGui::Draw();
}

void GSStartMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSStartMenu::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-startmenu.txt");
  Assert(m_gui);

  m_gui->GetElementByName("single-button")->SetCommand(Amju::OnSingleButton);
  m_gui->GetElementByName("multi-button")->SetCommand(Amju::OnMultiButton);
}

bool GSStartMenu::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSStartMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
