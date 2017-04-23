#include <AmjuFirst.h>
#include "GuiFactory.h"
#include "GuiButton.h"
#include "GuiCheck.h"
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
#include "GuiSlider.h"
#include "GuiDecInclude.h"
#include "GuiDecRotate.h"
#include "GuiDecScale.h"
#include "GuiDecTranslate.h"
#include "GuiDecColour.h"
#include "GuiDecAnimation.h"
#include <AmjuFinal.h>

namespace Amju
{
template <class T>
GuiElement* CreateT() { return new T; }

GuiFactory::GuiFactory()
{
  Add(GuiButton::NAME, &CreateT<GuiButton>);
  Add(GuiCheck::NAME, &CreateT<GuiCheck>);
  Add(GuiImage::NAME, &CreateT<GuiImage>);
  Add(GuiText::NAME, &CreateT<GuiText>);
  Add(GuiTextEdit::NAME, &CreateT<GuiTextEdit>);
  Add(GuiComposite::NAME, &CreateT<GuiComposite>);
  Add(GuiWindow::NAME, &CreateT<GuiWindow>);
  Add(GuiKeyboard::NAME, &CreateT<GuiKeyboard>);
  Add(GuiMenu::NAME, &CreateT<GuiMenu>);
  Add(GuiDialog::NAME, &CreateT<GuiDialog>);
  Add(GuiRect::NAME, &CreateT<GuiRect>);
  Add(GuiScroll::NAME, &CreateT<GuiScroll>);
  Add(GuiElastic::NAME, &CreateT<GuiElastic>);
  Add(GuiSlider::NAME, &CreateT<GuiSlider>);
  Add(GuiDecInclude::NAME, &CreateT<GuiDecInclude>);
  Add(GuiDecRotate::NAME, &CreateT<GuiDecRotate>);
  Add(GuiDecTranslate::NAME, &CreateT<GuiDecTranslate>);
  Add(GuiDecAnimation::NAME, &CreateT<GuiDecAnimation>);
  Add(GuiDecColour::NAME, &CreateT<GuiDecColour>);
  Add(GuiDecScale::NAME, &CreateT<GuiDecScale>);

  Add(GuiListBox::NAME, &CreateListBox);
  Add(GuiFileListBox::NAME, &CreateFileListBox);
  Add(GuiFileDialog::NAME, &CreateFileDialog);
  Add(GuiObjView::NAME, &CreateObjView);
  Add(GuiCalendar::NAME, &CreateCalendar);
  Add(GuiChart::NAME, &CreateChart);
  Add(GuiDataLineDisplay::NAME, &CreateDataLineDisplay);
}
}
