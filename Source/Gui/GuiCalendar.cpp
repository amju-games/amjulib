#include <TimePeriod.h>
#include "GuiCalendar.h"
#include "GuiText.h"
#include "DrawBorder.h"

namespace Amju
{
const char* GuiCalendar::NAME = "gui-calendar";

static const int ONE_DAY_IN_SECONDS = 60 * 60 * 24;
static const int ONE_WEEK_IN_SECONDS = ONE_DAY_IN_SECONDS * 7;

GuiElement* CreateCalendar()
{
  return new GuiCalendar;
}

GuiCalendar::GuiCalendar()
{
  m_focus = 0;
}

void GuiCalendar::Draw()
{
  GuiComposite::Draw();
  // Draw focus day
  if (m_focus)
  {
    GuiCalendarDayCell* dc = dynamic_cast<GuiCalendarDayCell*>(m_focus);
    Assert(dc);

    RCPtr<GuiText> text = new GuiText;
    text->SetIsMulti(true);
    text->SetTextSize(1.2f); // TODO CONFIG
    text->SetSize(Vec2f(0.8f, 0.1f)); // sets max line width
    text->SetLocalPos(m_focus->GetCombinedPos());
    text->SetText(dc->GetMainEventStr());
    text->SizeToText();
    text->SetFgCol(Colour(0, 0, 0 ,1));
    text->SetBgCol(Colour(1, 1, 1 ,1));
    text->SetDrawBg(true);

    text->Draw();
  }
}

const Vec2f& GuiCalendar::GetCellSize() const
{
  return m_cellSize;
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
  Clear();

  std::string MONTH[] = { "Jan", "Feb", "Mar", "April", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec" };

  Time today = Time::Now();
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  start.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  end.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  // Round up to end of week
  int daysToEndOfWeek = 7 - end.GetDayOfWeek();
  if (daysToEndOfWeek < 7)
  {
    end += TimePeriod(ONE_DAY_IN_SECONDS * daysToEndOfWeek);
  }

  int numdays = (end - start).ToSeconds() / ONE_DAY_IN_SECONDS;
  int numrows = numdays / 7;
  if ((numdays % 7) != 0)
  {
    numrows++;
  }
  Vec2f size = GetSize();
  float w = size.x / 7.0f;
  float h = (size.y - 0.1f) / (float)numrows;
  m_cellSize = Vec2f(w, h);

  int day = start.GetDayOfWeek() - 1;
  if (day < 0)
  {
    day += 7;
  }

  float y = -0.1f;
  float x = (float)day * w;
  for (int i = 0; i <= numdays; i++)
  {
    GuiCalendarDayCell* text = new GuiCalendarDayCell(this);
    text->SetTime(start);

    int dayOfMonth = start.GetDayOfMonth();
    int month = start.GetMonths() - 1; // 1-based
    std::string str = MONTH[month] + "\n" + ToString(dayOfMonth);
    text->SetText(str);

    if (start == today)
    {
      text->SetInverse(true);
    }

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

void GuiCalendar::SetFocusDay(GuiCalendarDayCell* focus)
{
  m_focus = focus;
}

bool GuiCalendar::OnCursorEvent(const CursorEvent& ce)
{
  GuiComposite::OnCursorEvent(ce);

  Rect r = GetRect(this);
  if (!r.IsPointIn(Vec2f(ce.x, ce.y)))
  {
    SetFocusDay(0);
  }
  return false;
}

GuiCalendarDayCell::GuiCalendarDayCell(GuiCalendar* parent) : m_timestamp(0)
{
  SetParent(parent);
  SetTextSize(1.0f); // TODO CONFIG
  SetSize(parent->GetCellSize()); 
  SetIsMulti(true);
  SetDrawBg(true);
}

void GuiCalendarDayCell::SetTime(Time t)
{
  m_timestamp = t;
}

Time GuiCalendarDayCell::GetTime() const
{
  return m_timestamp;
}

void GuiCalendarDayCell::Draw()
{
  GuiText::Draw(); // TODO for now
  DrawBorder(this, Colour(1, 1, 1, 1));
}

bool GuiCalendarDayCell::OnCursorEvent(const CursorEvent& ce)
{
  Rect r = GetRect(this);
  if (r.IsPointIn(Vec2f(ce.x, ce.y)))
  {
    // Show event
    ((GuiCalendar*)m_parent)->SetFocusDay(this);
  }
  return false;
}

const std::string GuiCalendarDayCell::GetMainEventStr() const
{
  return m_mainEventStr;
}

void GuiCalendarDayCell::SetMainEventStr(const std::string& str)
{
  m_mainEventStr = str;
}
}
