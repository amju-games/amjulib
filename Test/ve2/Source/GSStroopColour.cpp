#include <AmjuFirst.h>
#include "GSStroopColour.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiRect.h>
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

void GSStroopColour::OnActive()
{
  GSStroopBase::OnActive();
  m_colourRect = new GuiRect;
  m_colourRect->SetLocalPos(Vec2f(-0.5f, 0.4f));
  m_colourRect->SetSize(Vec2f(1.0f, 0.4f));

  m_moveElement = m_colourRect;
  m_moveElementOriginalPos = m_moveElement->GetLocalPos();
}

void GSStroopColour::OnDeactive()
{
  GSStroopBase::OnDeactive();
  m_colourRect = 0;
}

void GSStroopColour::Draw2d()
{
  // Draw coloured rect
  PushColour();
  MultColour(m_goodColour);
  m_colourRect->Draw();  
  PopColour();

  GSGui::Draw2d();
}
} // namespace
