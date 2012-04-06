#include "GuiFactory.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiText.h"
#include "GuiTextEdit.h"
#include "GuiComposite.h"
#include "GuiListBox.h"
#include "GuiWindow.h"

namespace Amju
{
GuiElement* CreateGuiButton() { return new GuiButton; }
GuiElement* CreateGuiImage() { return new GuiImage; }
GuiElement* CreateGuiText() { return new GuiText; }
GuiElement* CreateGuiTextEdit() { return new GuiTextEdit; }
GuiElement* CreateGuiComposite() { return new GuiComposite; }
GuiElement* CreateListBox() { return new GuiListBox; }
GuiElement* CreateWindow() { return new GuiWindow; }

GuiFactory::GuiFactory()
{
  Add(GuiButton::NAME, &CreateGuiButton);
  Add(GuiImage::NAME, &CreateGuiImage);
  Add(GuiText::NAME, &CreateGuiText);
  Add(GuiTextEdit::NAME, &CreateGuiTextEdit);
  Add(GuiComposite::NAME, &CreateGuiComposite);
  Add(GuiListBox::NAME, &CreateListBox);
  Add(GuiWindow::NAME, &CreateWindow);
}
}
