#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSCogTestMenu.h"
#include "GSFileUpdateCheck.h"
#include "GSLetterCancellation1.h"
#include "GSTitle.h"
#include "GSStroopWord.h"
#include "GSStroopColour.h"
#include "GSStroopColourWord.h"
#include "GSReactionTime.h"
#include "CogTestResults.h"

namespace Amju
{
static void OnCancelButton()
{
  // If we cancel, the session will not be complete so we should have to do the cog tests
  //  again ? 
  // No, the participant just skips the test, if that's what they want to do.

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  TheGSCogTestMenu::Instance()->ResetTests();
}

static void LetterSigmaTest()
{
  TheGSLetterCancellation1::Instance()->LoadConfig(AMJU_COG_TEST_SYMBOL_CAN, "letter-cancel-sigma.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation1::Instance());
}

static void LetterMTest()
{
  TheGSLetterCancellation1::Instance()->LoadConfig(AMJU_COG_TEST_LETTER_CAN, "letter-cancel-m.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation1::Instance());
}

static void StroopWord()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopWord::Instance());
}

static void StroopColour()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopColour::Instance());
}

static void StroopColourWord()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopColourWord::Instance());
}

static void ReactionTime()
{
  TheGame::Instance()->SetCurrentState(TheGSReactionTime::Instance());
}

static void Done()
{
  TheGame::Instance()->SetCurrentState(TheGSFileUpdateCheck::Instance());
  TheGSCogTestMenu::Instance()->ResetTests();
}

GSCogTestMenu::GSCogTestMenu()
{
  m_nextTest = 0;
}

void GSCogTestMenu::ResetTests()
{
  m_nextTest = 0;
}

void GSCogTestMenu::Update()
{
  GSGui::Update();
}

void GSCogTestMenu::Draw()
{
  GSGui::Draw();
}

void GSCogTestMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestMenu::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-cogtestmenu.txt");
  Assert(m_gui);

  // We display some text, which depends on the next test to administer.
  // The OK button then starts the next test.
  // TODO Set focus element, cancel element, command handlers
  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetIsFocusButton(true);

  GuiText* text = (GuiText*)GetElementByName(m_gui, "text1");
  Assert(text);

  // Set text and OK button command depending on next test to perform...
  // TODO Use localisation system so we can improve all the text.
  switch (m_nextTest)
  {
  case 0:
    text->SetText("Here comes Reaction Time test");
    ok->SetCommand(Amju::ReactionTime);
    break;

  case 5:
    text->SetText("Here comes STROOP COLOUR");
    ok->SetCommand(Amju::StroopColour);
    break;

  case 4:
    text->SetText("Here comes STROOP COLOUR WORD");
    ok->SetCommand(Amju::StroopColourWord);
    break;

  case 3:
    text->SetText("Here comes STROOP WORD");
    ok->SetCommand(Amju::StroopWord);
    break;

  case 2:
    text->SetText("Please take some tests before you start. This first one is a letter cancellation test. Click on all the M letters that you see. You have 3 minutes to complete this task.");
    ok->SetCommand(Amju::LetterMTest);
    break;

  case 1:
    text->SetText("In this test, click on all the sigma letters that you see.");
    ok->SetCommand(Amju::LetterSigmaTest);
    break;

  // etc

  default:
    text->SetText("Well done on doing all those tests. Now you can play the game.");
    ok->SetCommand(Amju::Done);
  }
  m_nextTest++;
}

} // namespace
