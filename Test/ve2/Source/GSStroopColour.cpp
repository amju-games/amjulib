#include <AmjuFirst.h>
#include "GSStroopColour.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include <SoundManager.h>
#include "ROConfig.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "GSMain.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "CogTestNag.h"
#include <AmjuFinal.h>

namespace Amju
{
void GSStroopColour::SetTest()
{
  GSStroopBase::SetTest();

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(false); 

  m_goodColour = COLOURS[m_indices[m_correctChoice]];
}

GSStroopColour::GSStroopColour()
{
  m_testName = "Stroop Colour ";
  m_testId = AMJU_COG_TEST_STROOP_COLOUR;
}

void GSStroopColour::ResetTest()
{
  GSStroopBase::ResetTest();

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(false);
}

void GSStroopColour::Draw2d()
{
  // Draw coloured rect
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  Rect r(-0.5f, 0.5f, 0, 0.4f);
  AmjuGL::SetColour(m_goodColour);
  DrawSolidRect(r);  
  PopColour();
  AmjuGL::PopAttrib();

  GSGui::Draw2d();
}
} // namespace
