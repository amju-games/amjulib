#ifndef GUI_CALENDAR_H_INCLUDED
#define GUI_CALENDAR_H_INCLUDED

#include <AmjuTime.h>
#include "GuiComposite.h"
#include "GuiText.h"

namespace Amju
{
class GuiCalendar;

// TODO just for now. Calendar is really a special kind of grid.
class GuiCalendarDayCell : public GuiText
{
public:
  GuiCalendarDayCell(GuiCalendar* parent);

  virtual void Draw();
  bool OnCursorEvent(const CursorEvent& ce);

  void SetTime(Time t);
  Time GetTime() const;

  // TODO Add events
  const std::string GetMainEventStr() const;
  void SetMainEventStr(const std::string& str);

protected:
  Time m_timestamp;
  std::string m_mainEventStr;
};

// Show grid of boxes containing dates, organised as rows of 7. So each row is one week.
class GuiCalendar : public GuiComposite
{
public:
  static const char* NAME;

  GuiCalendar();

  virtual bool Load(File*); 
  virtual void Draw();
  bool OnCursorEvent(const CursorEvent& ce);

  // Rounds time down to day.
  void SetStartEndDate(Time start, Time end);

  GuiCalendarDayCell* GetCell(Time day);

  const Vec2f& GetCellSize() const; 

  void SetFocusDay(GuiCalendarDayCell* focus);

  // TODO Set font, size, etc.

  void AddDayColumnHeadings();

private:
  Vec2f m_cellSize;
  GuiCalendarDayCell* m_focus;
};

GuiElement* CreateCalendar();
}

#endif
