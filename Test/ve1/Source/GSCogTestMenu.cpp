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
//  TheLurker::Instance()->Update();
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
//  TheLurker::Instance()->Draw();
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

  GetGuiSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(ch);

  m_gui = LoadGui("gui-cogtestmenu.txt");
  Assert(m_gui);

  // We display some text, which depends on the next test to administer.
  // The OK button then starts the next test.
  // TODO Set focus element, cancel element, command handlers
  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  if (cancel)
  {
    cancel->SetCommand(Amju::Done);
    cancel->SetIsCancelButton(true);
    cancel->SetText("skip!");
  }

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetIsFocusButton(true);

  GuiText* text = (GuiText*)GetElementByName(m_gui, "text1");
  Assert(text);

  // Set text and OK button command depending on next test to perform...
  // TODO Use localisation system so we can improve all the text.
  std::string str;
  CommandFunc func = 0;

  switch (m_nextTest)
  {
  case 0:
    str = "This is a reaction time game. Press the red button as soon as the alarm goes off!";
    func = Amju::ReactionTime;
    break;

  case 5:
    str = "Here comes STROOP COLOUR";
    func = Amju::StroopColour;
    break;

  case 4:
    str = "Here comes STROOP COLOUR WORD";
    func = Amju::StroopColourWord;
    break;

  case 3:
    str = "Here comes STROOP WORD";
    func = Amju::StroopWord;
    break;

  case 2:
    str = "Please take some tests before you start. This first one is a letter cancellation test. Click on all the M letters that you see. You have 3 minutes to complete this task.";
    func = Amju::LetterMTest;
    break;

  case 1:
    str = "In this test, click on all the sigma letters that you see.";
    func = Amju::LetterSigmaTest;
    break;

  // etc

  default:
    str = "Well done on doing all those tests. Now you can play the game.";
    func = Amju::Done;
  }
  m_nextTest++;

  text->SetText(str);
  ok->SetCommand(func);

  LurkMsg lm(str, Colour(1, 1, 1, 1), Colour(0.5f, 0, 0.5f, 0.5f), AMJU_CENTRE, 
    func);
  TheLurker::Instance()->Queue(lm);
}

} // namespace
