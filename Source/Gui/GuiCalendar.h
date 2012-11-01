#ifndef GUI_CALENDAR_H_INCLUDED
#define GUI_CALENDAR_H_INCLUDED

#include <AmjuTime.h>
#include "GuiComposite.h"

namespace Amju
{
// Show grid of boxes containing dates, organised as rows of 7. So each row is one week.
class GuiCalendar : public GuiComposite
{
public:
  static const char* NAME;

  virtual bool Load(File*); 

  // Rounds time down to day.
  void SetStartEndDate(Time start, Time end);

private:
  void AddDay(Time day);
};

GuiElement* CreateCalendar();
}

#endif
