#include "GuiChart.h"
#include "GuiDataLineDisplay.h" // TODO TEMP TEST

namespace Amju
{
const char* GuiChart::NAME = "gui-chart";

GuiChart::GuiChart()
{
}

GuiElement* CreateChart()
{
  // To make chart scroll, create a Scroll decorator which has a DataDisplay child.
  // Stick the whole thing in a Window.

  GuiChart* w = new GuiChart;
  GuiScroll* s = new GuiScroll;
  w->AddChild(s);
  return w;
}

GuiScroll* GuiChart::GetScroll()
{
  GuiElement* child = GetChild(0);
  Assert(child);
  GuiScroll* scr = dynamic_cast<GuiScroll*>(child);
  Assert(scr);
  return scr;
}

GuiDataDisplay* GuiChart::GetDataDisplay()
{
  GuiElement* grandchild = GetScroll()->GetChild(0);
  Assert(grandchild);
  GuiDataDisplay* dd = dynamic_cast<GuiDataDisplay*>(grandchild);
  Assert(dd);
  return dd;
}

bool GuiChart::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  GetScroll()->SetSize(GetSize()); // TODO do we need this ?
  GetDataDisplay()->SetSize(GetSize());

  // TODO Set type of data display
  // TODO Use factory for this
  GetScroll()->AddChild(new GuiDataLineDisplay);

  return true;
}

void GuiChart::Clear()
{
  // Clear data, not the child of this object
}

}

