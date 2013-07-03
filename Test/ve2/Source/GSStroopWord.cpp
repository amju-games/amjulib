#include <AmjuFirst.h>
#include "GSStroopWord.h"
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
void GSStroopWord::SetTest()
{
  GSStroopBase::SetTest();
}

GSStroopWord::GSStroopWord()
{
  m_testName = "Stroop Word";
  m_testId = AMJU_COG_TEST_STROOP_WORD;
}
} // namespace
