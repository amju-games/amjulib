#include <AmjuFirst.h>
#include "GSStroopColourWord.h"
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
#include "GameMode.h"
#include "GSCogResults.h"
#include "CogTestNag.h"
#include "HeartCount.h"
#include "ObjectUpdater.h"
#include <AmjuFinal.h>

namespace Amju
{
void GSStroopColourWord::SetTest()
{
  GSStroopBase::SetTest();

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  int incorrect = m_correctChoice;
  while (incorrect == m_correctChoice)
  {
    incorrect = rand() % 4;
  }
  word->SetFgCol(COLOURS[m_indices[m_correctChoice]]); // colour is correct
  word->SetText(WORDS[m_indices[incorrect]]); // text is incorrect
}

GSStroopColourWord::GSStroopColourWord()
{
  m_testName = "Stroop Colour Word";
  m_testId = AMJU_COG_TEST_STROOP_COLOUR_WORD;
}

} // namespace
