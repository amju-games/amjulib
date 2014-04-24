#include <AmjuFirst.h>
#include "GuiFactory.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiText.h"
#include "GuiTextEdit.h"
#include "GuiComposite.h"
#include "GuiListBox.h"
#include "GuiFileListBox.h"
#include "GuiWindow.h"
#include "GuiKeyboard.h"
#include "GuiMenu.h"
#include "GuiDialog.h"
#include "GuiRect.h"
#include "GuiScroll.h"
#include "GuiFileDialog.h"
#include "GuiObjView.h"
#include "GuiCalendar.h"
#include "GuiDataBarDisplay.h"
#include "GuiChart.h"
#include "GuiDataLineDisplay.h"
#include "GuiElastic.h"
#include <AmjuFinal.h>

namespace Amju
{
GuiElement* CreateGuiButton() { return new GuiButton; }
GuiElement* CreateGuiImage() { return new GuiImage; }
GuiElement* CreateGuiText() { return new GuiText; }
GuiElement* CreateGuiTextEdit() { return new GuiTextEdit; }
GuiElement* CreateGuiComposite() { return new GuiComposite; }
GuiElement* CreateWindow() { return new GuiWindow; }
GuiElement* CreateKb() { return new GuiKeyboard; }
GuiElement* CreateMenu() { return new GuiMenu; }
GuiElement* CreateDialog() { return new GuiDialog; }
GuiElement* CreateRect() { return new GuiRect; }
GuiElement* CreateScroll() { return new GuiScroll; }
GuiElement* CreateElastic() { return new GuiElastic; }

GuiFactory::GuiFactory()
{
  Add(GuiButton::NAME, &CreateGuiButton);
  Add(GuiImage::NAME, &CreateGuiImage);
  Add(GuiText::NAME, &CreateGuiText);
  Add(GuiTextEdit::NAME, &CreateGuiTextEdit);
  Add(GuiComposite::NAME, &CreateGuiComposite);
  Add(GuiListBox::NAME, &CreateListBox);
  Add(GuiWindow::NAME, &CreateWindow);
  Add(GuiKeyboard::NAME, &CreateKb);
  Add(GuiMenu::NAME, &CreateMenu);
  Add(GuiDialog::NAME, &CreateDialog);
  Add(GuiRect::NAME, &CreateRect);
  Add(GuiFileListBox::NAME, &CreateFileListBox);
  Add(GuiScroll::NAME, &CreateScroll);
  Add(GuiFileDialog::NAME, &CreateFileDialog);
  Add(GuiObjView::NAME, &CreateObjView);
  Add(GuiCalendar::NAME, &CreateCalendar);
  Add(GuiChart::NAME, &CreateChart);
  Add(GuiDataLineDisplay::NAME, &CreateDataLineDisplay);
  Add(GuiElastic::NAME, &CreateElastic);
}
}