#include <AmjuGL.h>
#include <Screen.h>
#include <Game.h>
#include <GuiButton.h>
#include <SoundManager.h>
#include "GSCogTestMenu.h"
#include "GSFileUpdateCheck.h"
#include "GSLetterCancellation.h"
#include "GSTitle.h"
#include "GSStroopWord.h"
#include "GSStroopColour.h"
#include "GSStroopColourWord.h"
#include "GSReactionTime.h"
#include "CogTestResults.h"
#include "Ve1Character.h"
#include "Ve1SceneGraph.h"
#include "AvatarManager.h"
#include "LurkMsg.h"
#include "GameMode.h"
#include "ROConfig.h"
#include "GameConsts.h"

namespace Amju
{
/*
static void OnCancelButton()
{
  // If we cancel, the session will not be complete so we should have to do the cog tests
  //  again ? 
  // No, the participant just skips the test, if that's what they want to do.

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  TheGSCogTestMenu::Instance()->ResetTests();
}
*/

static void NoPrac()
{
  TheGSCogTestMenu::Instance()->DoPractice(false);
}

static void YesPrac()
{
  TheGSCogTestMenu::Instance()->DoPractice(true);
}

static void AskPractice()
{
  TheGSCogTestMenu::Instance()->AskPractice();
}

static void LetterSigmaTest()
{
  TheGSLetterCancellation::Instance()->LoadConfig(AMJU_COG_TEST_SYMBOL_CAN, "letter-cancel-sigma.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation::Instance());
}

static void LetterMTest()
{
  TheGSLetterCancellation::Instance()->LoadConfig(AMJU_COG_TEST_LETTER_CAN, "letter-cancel-m.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation::Instance());
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
  ResetTests();
  m_func = 0;
  m_isPrac = false;
  m_showLurk = true;
}

void GSCogTestMenu::ResetTests()
{
  m_nextTest = ROConfig()->GetInt("cogtest-first-test", 0); // For testing
}

void GSCogTestMenu::Update()
{
  GSGui::Update();
}

void GSCogTestMenu::Draw()
{
  DrawCogTestBg();
}

void GSCogTestMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestMenu::AdvanceToNextTest()
{
  m_nextTest++;
}

bool GSCogTestMenu::IsDoingTests()
{
  bool b = DoCogTests() && m_nextTest < 6;
  return b;
}

void GSCogTestMenu::OnActive()
{
  // TODO Fade vol down to v quiet
//  TheSoundManager::Instance()->StopSong();
//  TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-new-cogtest"));

  //GSGui::OnActive();
  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));
  LoadCogTestBg();
//  if (!m_bgImage.OpenAndLoad("cogbgimage.txt"))
//  {
//std::cout << "Failed to load GUI bg image!\n";
//    Assert(0);
//  }

  // Add a character, so it looks like she is talking to the player.
  // This was confusing for participant, so drop this idea.
/*
  Ve1Character* ch = new Ve1Character(0);
  // TODO CONFIG - these depend on the contents of charlist.txt!
  TheAvatarManager::Instance()->SetAvatar(4, ch);
  TheAvatarManager::Instance()->SetTexture(7, ch);

  // TODO CONFIG position character
  Vec3f pos(-25.0f, -25.0f, -100.0f);

  Matrix m;
  m.Translate(pos);
  ch->SetLocalTransform(m);

  ClearGuiSceneGraph();
  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(ch);
*/

  m_gui = LoadGui("gui-cogtestmenu.txt");
  Assert(m_gui);

  // Set text and OK button command depending on next test to perform...
  // TODO Use localisation system so we can improve all the text.
  /*
  TODO
  std::string strs[] = 
  {
  };
  CommandFunc funcs[] = 
  {
  };
  */
  m_isPrac = false;
  std::string str;

  switch (m_nextTest)
  {
  case 0:
    str = "OK, thanks! This first one is a reaction time test. Please press the big red button as soon as the alarm goes off!";
    m_func = Amju::ReactionTime;
    break;

  case 1:
    str = "This next one is called a letter cancellation test. Please click on all the M letters that you see. You have 3 minutes to complete this task.";
    m_func = Amju::LetterMTest;
    break;

  case 2:
    {
    str = "This time, click on all the sigma letters that you see. They look like this..."; 
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    GuiText* text = new GuiText;
    text->SetFont("font2d/symbol-font.font");
    text->SetTextSize(4.5f); // TODO CONFIG
    text->SetSize(Vec2f(1.6f, 0.5f)); 
    text->SetText("S");
    text->SizeToText();
    text->SetFgCol(LURK_FG);
    lm.Set(text, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    str = "It's the same rules as before, but this time find the sigma letters!";

    m_func = Amju::LetterSigmaTest;
    }
    break;

  case 3:
    str = "This next one is called a Stroop Colour test. I will show you a coloured rectangle. Please click on the button which says the colour of the rectangle.";
    m_func = Amju::StroopColour;
    break;

  case 4:
    str = "I have two more for you. This is called the Stroop Word test. I will show you a word. Please click on the button which is the same as the word. Easy!";
    m_func = Amju::StroopWord;
    break;

  case 5:
    {
      str = "This is the last one. It's called the Stroop Colour Word test.";
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
      TheLurker::Instance()->Queue(lm);
    }
    str = "I will show you a word like before. But this time, the words are coloured. "
      "You have to click the button for the colour, not the word. It's quite tricky!";
    m_func = Amju::StroopColourWord;
    break;

  default:
    str = "Well done on doing all those tests! Now please explore this planet, and find the missing space ship parts!";
    m_func = Amju::Done;
  }

  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, Amju::AskPractice);
  TheLurker::Instance()->Queue(lm);
}

void GSCogTestMenu::AskPractice()
{
  std::string q("Would you like to have a practice go first ?");
  TheLurker::Instance()->ShowYesNo(q, LURK_FG, LURK_BG, Amju::NoPrac, Amju::YesPrac);
}

void GSCogTestMenu::SetIsPrac(bool b)
{
  m_isPrac = b;
}

bool GSCogTestMenu::IsPrac() const
{
  return m_isPrac;
}

void GSCogTestMenu::DoPractice(bool b)
{
  if (b)
  {
    SetIsPrac(b);
  }
  m_func();
}

} // namespace
