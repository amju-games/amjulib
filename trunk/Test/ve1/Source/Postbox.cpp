#include "Postbox.h"
#include <GuiMenu.h>

namespace Amju
{
void Postbox::SetMenu(GuiMenu* menu)
{
  menu->AddItem(new GuiMenuItem("Send a message..."));
}
}

