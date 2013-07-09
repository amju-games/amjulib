#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include "GSPauseCogTest.h"
//#include "GSMain.h"
//#include "GSOptions.h"
//#include "GSTitle.h"
//#include "GSQuitGame.h"
//#include "LocalPlayer.h"
//#include "GSCalendar.h"
//#include "GSAchievements.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnResumeButton()
{
  TheGSPauseCogTest::Instance()->GoBack();
}

GSPauseCogTest::GSPauseCogTest()
{
}

void GSPauseCogTest::Update()
{
  GSGui::Update();

}

void GSPauseCogTest::Draw()
{
  GSGui::Draw();

}

void GSPauseCogTest::Draw2d()
{
  GSGui::Draw2d();
}

void GSPauseCogTest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-pause-cogtest.txt");
  Assert(m_gui);

  m_gui->GetElementByName("resume-button")->SetCommand(Amju::OnResumeButton);
}
} // namespace
