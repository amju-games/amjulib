#include "GuiFactory.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiText.h"
#include "GuiComposite.h"

namespace Amju
{
GuiElement* CreateGuiButton() { return new GuiButton; }
GuiElement* CreateGuiImage() { return new GuiImage;}
GuiElement* CreateGuiText() { return new GuiText;}
GuiElement* CreateGuiComposite() { return new GuiComposite;}

GuiFactory::GuiFactory()
{
  Add(GuiButton::NAME, &CreateGuiButton);
  Add(GuiImage::NAME, &CreateGuiImage);
  Add(GuiText::NAME, &CreateGuiText);
  Add(GuiComposite::NAME, &CreateGuiComposite);
}
}
