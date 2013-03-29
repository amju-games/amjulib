#include <AmjuFirst.h>
#include "GuiChart.h"
#include "GuiFactory.h"
#include "GuiDataLineDisplay.h" // TODO TEMP TEST
#include <AmjuFinal.h>

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

  std::string displayType;
  if (!f->GetDataLine(&displayType))
  {
    f->ReportError("Gui Chart expected display type");
    return false;
  }
  GuiElement* elem = TheGuiFactory::Instance()->Create(displayType);
  if (!elem)
  {
    f->ReportError("Gui Chart: unexpected display type: " + displayType);
    return false;
  }
  GetScroll()->AddChild(elem);
  if (!elem->Load(f))
  {
    f->ReportError("Failed to load Gui Chart as display type failed to load");
    return false;
  }
  //GetDataDisplay()->SetSize(GetSize());

  return true;
}

void GuiChart::Clear()
{
  // Clear data, not the child of this object
}

}

