#include <GuiScroll.h>
#include <GuiText.h>
#include "GuestbookWindow.h"

namespace Amju
{
GuestbookWindow* CreateGuestbookWindow()
{
  GuestbookWindow* w = new GuestbookWindow;
  GBDisplay* gbd = new GBDisplay;
  GuiScroll* s = new GuiScroll;
  w->AddChild(s);
  s->AddChild(gbd);

  return w;
}

// Call once when initialising GB 
void GBDisplay::Init(Player* player)
{
  // Get messages for the current player, add GUI elements to GBDisplay

  // Add one row
  GuiComposite* comp = new GuiComposite;
  // Add message
  GuiText* text = new GuiText;
  text->SetText("This is a test msg");
  comp->AddChild(text);
  // Set pos - y is number of rows
  comp->SetSizeFromChildren();

  AddChild(comp);
  
}

}

