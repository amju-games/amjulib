#include <TimePeriod.h>
#include "GuiCalendar.h"
#include "GuiText.h"

namespace Amju
{
const char* GuiCalendar::NAME = "gui-calendar";

static const int ONE_DAY_IN_SECONDS = 60 * 60 * 24;

GuiElement* CreateCalendar()
{
  return new GuiCalendar;
}

bool GuiCalendar::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // Add top row of days - TODO Set start day
  // TODO localise
  // TODO long/short day names
  std::string DAYS[] = { "Mon", "Tues", "Weds", "Thurs", "Fri", "Sat", "Sun" };

  GuiComposite* toprow = new GuiComposite;
  Vec2f size = GetSize();
  toprow->SetSize(Vec2f(size.x, 0.1f));
  float w = size.x / 7.0f;

  AddChild(toprow);
  for (int i = 0; i < 7; i++)
  {
    GuiText* text = new GuiText;
    // TODO Colours
    text->SetTextSize(1.0f); // TODO CONFIG
    text->SetSize(Vec2f(w, 0.1f)); // assume single line
    text->SetText(DAYS[i]);
    text->SetLocalPos(Vec2f(w * (float)i, 0));
    text->SetFgCol(Colour(1, 1, 1, 1));
    Colour bg(0, 0, 1, 1);
    if ((i % 1) != 0)
    {
      bg = Colour(0.2f, 0.2f, 1, 1);
    }

    text->SetBgCol(bg);
    text->SetDrawBg(true);
    toprow->AddChild(text);
  }

  return true;
}

void GuiCalendar::SetStartEndDate(Time start, Time end)
{
  start.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  end.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  int numdays = (end - start).ToSeconds() / ONE_DAY_IN_SECONDS;
  int numrows = numdays / 7;
  if ((numdays % 7) != 0)
  {
    numrows++;
  }
  Vec2f size = GetSize();
  float w = size.x / 7.0f;
  float h = (size.y - 0.1f) / (float)numrows;
  Vec2f cellSize(w, h);

  // TODO split into rows ?
  for (int i = 0; i < numdays; i++)
  {
    GuiText* text = new GuiText;
    text->SetTextSize(1.0f); // TODO CONFIG
    text->SetSize(cellSize); 
    text->SetText(ToString(i));
    text->SetLocalPos(Vec2f(w * (float)i, h * (float)i));
    text->SetFgCol(Colour(1, 1, 1, 1));
    Colour bg(0, 0, 1, 1);
    if (i % 1)
    {
      bg = Colour(0.2f, 0.2f, 1, 1);
    }

    text->SetBgCol(bg);
    text->SetDrawBg(true);
    
    AddChild(text);
  }
}

void GuiCalendar::AddDay(Time day)
{
}

}
