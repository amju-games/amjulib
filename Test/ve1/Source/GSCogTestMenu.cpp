#include <AmjuGL.h>
#include <Screen.h>
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
#include "Ve1Character.h"
#include "Ve1SceneGraph.h"
#include "AvatarManager.h"
#include "LurkMsg.h"
#include "GameMode.h"

namespace Amju
{
static const Colour FG_COLOUR(1, 1, 1, 1);
static const Colour BG_COLOUR(0.5f, 0, 0.5f, 0.5f);

static void OnCancelButton()
{
  // If we cancel, the session will not be complete so we should have to do the cog tests
  //  again ? 
  // No, the participant just skips the test, if that's what they want to do.

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  TheGSCogTestMenu::Instance()->ResetTests();
}

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
  ResetTests();
  m_func = 0;
  m_isPrac = false;
  m_showLurk = true;
}

void GSCogTestMenu::ResetTests()
{
  m_nextTest = 3; // TODO TEMP TEST
}

void GSCogTestMenu::Update()
{
  GSGui::Update();
}

void GSCogTestMenu::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f; 
  const float FAR_PLANE = 3000.0f;
  static const float aspect = (float)Screen::X() / (float)Screen::Y();
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::PushMatrix();

  AmjuGL::LookAt(0, 2.0f, 8.0f,   0, 0, 0,  0, 1.0f, 0);

  GetGuiSceneGraph()->Draw();
  AmjuGL::PopMatrix();
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
  //GSGui::OnActive();
  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));
  if (!m_bgImage.OpenAndLoad("cogbgimage.txt"))
  {
std::cout << "Failed to load GUI bg image!\n";
    Assert(0);
  }

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
    str = "This is a reaction time game. What you have to do is press the big red button as soon as the alarm goes off!";
    m_func = Amju::ReactionTime;
    break;

  case 1:
    str = "This next one is called a letter cancellation test. Please click on all the M letters that you see. You have 3 minutes to complete this task.";
    m_func = Amju::LetterMTest;
    break;

  case 2:
    {
    str = "This time, click on all the sigma letters that you see. They look like this..."; 
    LurkMsg lm(str, FG_COLOUR, BG_COLOUR, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    GuiText* text = new GuiText;
    text->SetFont("font2d/symbol-font.font");
    text->SetTextSize(4.5f); // TODO CONFIG
    text->SetSize(Vec2f(1.6f, 0.5f)); 
    text->SetText("S");
    text->SizeToText();
    text->SetFgCol(FG_COLOUR);
    lm.Set(text, FG_COLOUR, BG_COLOUR, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    str = "It's the same rules as before, but this time find the sigma letters!";

    m_func = Amju::LetterSigmaTest;
    }
    break;

  case 3:
    str = "This next one is called a Stroop Colour test. Click on the button which says the colour of the coloured rectangle.";
    m_func = Amju::StroopColour;
    break;

  case 4:
    str = "Nearly finished. This is called the Stroop Word test.";
    m_func = Amju::StroopWord;
    break;

  case 5:
    str = "This is the last one. It's called the Stroop Colour Word test.";
    m_func = Amju::StroopColourWord;
    break;

  default:
    str = "Well done on doing all those tests! Now you can carry on playing the game!";
    m_func = Amju::Done;
  }

  LurkMsg lm(str, FG_COLOUR, BG_COLOUR, AMJU_CENTRE, Amju::AskPractice);
  TheLurker::Instance()->Queue(lm);
}

void GSCogTestMenu::AskPractice()
{
  std::string q("Would you like to have a practice go first ?");
  TheLurker::Instance()->ShowYesNo(q, FG_COLOUR, BG_COLOUR, Amju::NoPrac, Amju::YesPrac);
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
