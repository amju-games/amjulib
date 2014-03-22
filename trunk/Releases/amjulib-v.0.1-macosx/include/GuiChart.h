#ifndef GUI_CHART_H_INCLUDED
#define GUI_CHART_H_INCLUDED

#include <vector>
#include "GuiWindow.h"
#include "GuiScroll.h"
#include "GuiDataDisplay.h"

namespace Amju
{
// GuiChart is a Window containing a Scroll control, which contains a 
//  DataDisplay. The GuiChart has ptr to the data, which is displayed by
//  different DataDisplays in different ways. E.g. line, bar, etc.
class GuiChart : public GuiWindow
{
protected:
  GuiChart();
  friend GuiElement* CreateChart();

public:
  static const char* NAME;

  // Convenient access to innards
  GuiScroll* GetScroll();
  GuiDataDisplay* GetDataDisplay();

  virtual bool Load(File*);
  virtual void Clear(); // Clear data, not the child of this object

};

GuiElement* CreateChart();
}

#endif

