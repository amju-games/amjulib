#pragma once

#include <GuiWindow.h>

namespace Amju
{
class Player;

// The insides of a GuestbookWindow, which is presented inside a scrolling window
class GBDisplay : public GuiComposite
{
public:
  void GBDisplay::Init(Player* player);

};

class GuestbookWindow : public GuiWindow
{
public:
  // TODO Are these necessary?? Maybe but protected or private?
  // Convenient access to innards
  GuiScroll* GetScroll();
  GBDisplay* GetGBDisplay();
};

GuestbookWindow* CreateGuestbookWindow();
}
