#ifndef GUI_LIST_BOX_H_INCLUDED
#define GUI_LIST_BOX_H_INCLUDED

#include "GuiComposite.h"
#include "GuiText.h"

namespace Amju
{
class GuiListBox : public GuiComposite
{
public:
  static const char* NAME;

  void AddItem(GuiText* text);  
};
}

#endif

