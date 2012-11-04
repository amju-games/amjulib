#ifndef GUI_CALENDAR_H_INCLUDED
#define GUI_CALENDAR_H_INCLUDED

#include <AmjuTime.h>
#include "GuiComposite.h"
#include "GuiText.h"

namespace Amju
{
// TODO just for now. Calendar is really a special kind of grid.
class GuiCalendarDayCell : public GuiText
{
public:
  GuiCalendarDayCell();

  void SetTime(Time t);
  Time GetTime() const;

  // TODO Add events

protected:
  Time m_timestamp;
};

// Show grid of boxes containing dates, organised as rows of 7. So each row is one week.
class GuiCalendar : public GuiComposite
{
public:
  static const char* NAME;

  virtual bool Load(File*); 

  // Rounds time down to day.
  void SetStartEndDate(Time start, Time end);

  GuiCalendarDayCell* GetCell(Time day);

  // TODO Set font

private:
};

GuiElement* CreateCalendar();
}

#endif
