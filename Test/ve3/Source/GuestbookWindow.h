#pragma once

#include <GuiWindow.h>
#include <GuiSCroll.h>
#include "MsgManager.h"

namespace Amju
{
class Player;

// The insides of a GuestbookWindow, which is presented inside a scrolling window
class GBDisplay : public GuiComposite
{
public:
  void Init(const MsgManager::Msgs&);
};

class GuestbookWindow : public GuiWindow
{
public:
  // TODO Are these necessary?? Maybe but protected or private?
  // Convenient access to innards
  GuiScroll* GetScroll();
  GBDisplay* GetGBDisplay();
};

GuiElement* CreateGuestbookWindow();
}
