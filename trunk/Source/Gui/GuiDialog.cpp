#include <DrawRect.h>
#include <AmjuGL.h>
#include "GuiDialog.h"

namespace Amju
{
const char* GuiDialog::NAME = "gui-dialog";

bool GuiDialog::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  if (!LoadChildren(f))
  {
    return false;
  }
  return true;
}

void GuiDialog::Draw()
{
  // Draw bg - TODO colour, texture ? Rounded corners etc ?
  Rect r = GetRect(this);
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawSolidRect(r);
  AmjuGL::PopAttrib(); 

  GuiComposite::Draw();
}
}

