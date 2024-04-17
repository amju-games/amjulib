#include <AmjuFirst.h>
#include "GuiButton.h"
#include "GuiCalendar.h"
#include "GuiChart.h"
#include "GuiCheck.h"
#include "GuiComposite.h"
#include "GuiDataBarDisplay.h"
#include "GuiDataLineDisplay.h"
#include "GuiDecInclude.h"
#include "GuiDecRotate.h"
#include "GuiDecScale.h"
#include "GuiDecTranslate.h"
#include "GuiDecColour.h"
#include "GuiDecAnimation.h"
#include "GuiDecShader.h"
#include "GuiDialog.h"
#include "GuiElastic.h"
#include "GuiFactory.h"
#include "GuiFileDialog.h"
#include "GuiFileListBox.h"
#include "GuiImage.h"
#include "GuiKeyboard.h"
#include "GuiListBox.h"
#include "GuiMenu.h"
#include "GuiObjView.h"
#include "GuiPoly.h"
#include "GuiRect.h"
#include "GuiText.h"
#include "GuiTextEdit.h"
#include "GuiScroll.h"
#include "GuiSlider.h"
#include "GuiSpline.h"
#include "GuiSprite.h"
#include "GuiWindow.h"
#include <AmjuFinal.h>

namespace Amju
{
template <class T>
GuiElement* CreateT() { return new T; }

GuiFactory::GuiFactory()
{
  Add(GuiButton::NAME, &CreateT<GuiButton>);
  Add(GuiCheck::NAME, &CreateT<GuiCheck>);
  Add(GuiComposite::NAME, &CreateT<GuiComposite>);
  Add(GuiDecAnimation::NAME, &CreateT<GuiDecAnimation>);
  Add(GuiDecColour::NAME, &CreateT<GuiDecColour>);
  Add(GuiDecInclude::NAME, &CreateT<GuiDecInclude>);
  Add(GuiDecRotate::NAME, &CreateT<GuiDecRotate>);
  Add(GuiDecScale::NAME, &CreateT<GuiDecScale>);
  Add(GuiDecShader::NAME, &CreateT<GuiDecShader>);
  Add(GuiDecTranslate::NAME, &CreateT<GuiDecTranslate>);
  Add(GuiDialog::NAME, &CreateT<GuiDialog>);
  Add(GuiElastic::NAME, &CreateT<GuiElastic>);
  Add(GuiFlush::NAME, &CreateT<GuiFlush>);
  Add(GuiImage::NAME, &CreateT<GuiImage>);
  Add(GuiKeyboard::NAME, &CreateT<GuiKeyboard>);
  Add(GuiMenu::NAME, &CreateT<GuiMenu>);
  Add(GuiPoly::NAME, &CreateT<GuiPoly>);
  Add(GuiText::NAME, &CreateT<GuiText>);
  Add(GuiTextEdit::NAME, &CreateT<GuiTextEdit>);
  Add(GuiWindow::NAME, &CreateT<GuiWindow>);
  Add(GuiRect::NAME, &CreateT<GuiRect>);
  Add(GuiScroll::NAME, &CreateT<GuiScroll>);
  Add(GuiSlider::NAME, &CreateT<GuiSlider>);
  Add(GuiSprite::NAME, &CreateT<GuiSprite>);
  Add(GuiSpline::NAME, &CreateT<GuiSpline>);

  Add(GuiListBox::NAME, &CreateListBox);
  Add(GuiFileListBox::NAME, &CreateFileListBox);
  Add(GuiFileDialog::NAME, &CreateFileDialog);
  Add(GuiObjView::NAME, &CreateObjView);
  Add(GuiCalendar::NAME, &CreateCalendar);
  Add(GuiChart::NAME, &CreateChart);
  Add(GuiDataLineDisplay::NAME, &CreateDataLineDisplay);
}
}
