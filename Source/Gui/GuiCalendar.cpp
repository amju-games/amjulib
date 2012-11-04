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
  std::string DAY[] = { "Mon", "Tues", "Weds", "Thurs", "Fri", "Sat", "Sun" };

  GuiComposite* toprow = new GuiComposite;
  Vec2f size = GetSize();
  toprow->SetSize(Vec2f(size.x, 0.1f));
  float w = size.x / 7.0f;

  AddChild(toprow);
  for (int i = 0; i < 7; i++)
  {
    GuiText* text = new GuiText;
    // TODO Colours
    // TODO Font
    text->SetTextSize(1.0f); // TODO CONFIG
    text->SetSize(Vec2f(w, 0.1f)); // assume single line
    text->SetText(DAY[i]);
    text->SetLocalPos(Vec2f(w * (float)i, 0));
    text->SetFgCol(Colour(1, 1, 1, 1));
    Colour bg(0, 0, 1, 1);
    if ((i % 2) != 0)
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
  std::string MONTH[] = { "Jan", "Feb", "Mar", "April", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec" };

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

  int day = start.GetDayOfWeek() - 1;
  if (day < 0)
  {
    day += 7;
  }

  float y = -0.1f;
  float x = (float)day * w;
  for (int i = 0; i <= numdays; i++)
  {
    GuiCalendarDayCell* text = new GuiCalendarDayCell;
    text->SetTextSize(1.0f); // TODO CONFIG
    text->SetSize(cellSize); 
    text->SetIsMulti(true);
    text->SetTime(start);

    int dayOfMonth = start.GetDayOfMonth();
    int month = start.GetMonths() - 1; // 1-based
    std::string str = MONTH[month] + "\n" + ToString(dayOfMonth);
    text->SetText(str);
//std::cout << start.ToString() << "\n";

    start += ONE_DAY_IN_SECONDS;
    text->SetLocalPos(Vec2f(x, y));
    day++;
    if (day >= 7)
    {
      day = 0;
      x = 0;
      y -= h;
    }
    else
    {
      x += w;
    }

    text->SetFgCol(Colour(1, 1, 1, 1));
    Colour bg(0, 0, 1, 1);
    if ((i % 2) == 1)
    {
      bg = Colour(0.2f, 0.2f, 1, 1);
    }

    text->SetBgCol(bg);
    text->SetDrawBg(true);
    
    AddChild(text);
  }
}

GuiCalendarDayCell* GuiCalendar::GetCell(Time t)
{
  t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  int n = GetNumChildren();
  for (int i = 0; i < n; i++)
  {
    GuiElement* e = GetChild(i);
    GuiCalendarDayCell* dc = dynamic_cast<GuiCalendarDayCell*>(e);
    if (dc && dc->GetTime() == t) 
    {
      return dc;   
    }
  }
  return 0;
}

GuiCalendarDayCell::GuiCalendarDayCell() : m_timestamp(0)
{
}

void GuiCalendarDayCell::SetTime(Time t)
{
  m_timestamp = t;
}

Time GuiCalendarDayCell::GetTime() const
{
  return m_timestamp;
}
}
