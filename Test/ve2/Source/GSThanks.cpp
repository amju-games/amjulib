#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include <Game.h>
#include "GSThanks.h"
#include "GSCalendar.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnThanksOk()
{
  TheGSThanks::Instance()->GoBack();
}

static void OnThanksCal()
{
  TheGame::Instance()->SetCurrentState(TheGSCalendar::Instance());
}

GSThanks::GSThanks()
{
}

void GSThanks::Update()
{
  GSGui::Update();

}

void GSThanks::Draw()
{
  GSGui::Draw();

}

void GSThanks::Draw2d()
{
  GSGui::Draw2d();
}

void GSThanks::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-thanks.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "thanks-ok-button");
  ok->SetCommand(OnThanksOk);

  GuiButton* cal = (GuiButton*)GetElementByName(m_gui, "thanks-calendar-button");
  cal->SetCommand(OnThanksCal);

  GuiText* text = (GuiText*)GetElementByName(m_gui, "long-text");
  text->SetText("Thanks for participating in this research project! I hope you find it enjoyable.\n\nYou are asked to play this game a total of 11 times, over the course of 4 weeks. I will show you a calendar of when to play.\n\nIn the game, you are asked to take some tests. This is part of the research project, and I am sorry if it's a bit boring.\n\nWould you like to see the calendar now?");
}

} // namespace
