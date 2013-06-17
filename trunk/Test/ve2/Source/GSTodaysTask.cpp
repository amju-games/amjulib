#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSTodaysTask.h"
#include "GSStartGame.h"
#include "ResearchCalendar.h"
#include "GameMode.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnTodaysTaskOk()
{
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

GSTodaysTask::GSTodaysTask()
{
}

void GSTodaysTask::Update()
{
  GSGui::Update();
}

void GSTodaysTask::Draw()
{
  GSGui::Draw();
}

void GSTodaysTask::Draw2d()
{
  GSGui::Draw2d();
}

void GSTodaysTask::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-todaystask.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnTodaysTaskOk);

  int d = TheResearchCalendar::Instance()->GetDayOnPlanet();
  Assert(d > 0);
  //Assert(d <= 8); //????
  std::string str = "Day " + ToString(d) + "\non the island";

  GuiText* dayN = (GuiText*)GetElementByName(m_gui, "date-text");
  dayN->SetText(str);

  const int FUEL_CELL_OBJECTIVE[8] = { 10, 20, 25, 30, 35, 40, 50, 100 };
  int f = FUEL_CELL_OBJECTIVE[d - 1];

  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_SINGLE)
  {
    str = "Find food for yourself!";
  }
  else if (gm == AMJU_MODE_MULTI)
  {
    str = "Find food for your friends on the island!";
  }
  else
  {
    Assert(0);
    str = "";
  }

  GuiText* obj = (GuiText*)GetElementByName(m_gui, "obj-text");
  obj->SetText(str);
}

} // namespace
