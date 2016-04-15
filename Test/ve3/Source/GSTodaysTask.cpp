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
static void OnTodaysTaskOk(GuiElement*)
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


  std::string str;
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_SINGLE)
  {
    str = "Your goal is to find treasure! Keep yourself alive by eating food you find!";
  }
  else if (gm == AMJU_MODE_MULTI)
  {
    str = "Your goal is to find treasure, and to keep your friends alive by giving them food!";
  }
  else
  {
    Assert(0);
    str = "";
  }
  str += "\n\nTo move around, click the ground where you want to go.\n\n"
    "You can change the way you look - click on your player, and choose \"Change Look\"."
    "\n\nGood luck!"; 
  GuiText* obj = (GuiText*)GetElementByName(m_gui, "obj-text");
  obj->SetText(str);
}

} // namespace
