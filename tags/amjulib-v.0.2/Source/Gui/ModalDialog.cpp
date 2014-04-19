#include <Game.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiFileDialog.h>
#include "ModalDialog.h"

namespace Amju
{
void DoModalDialog(Dialog* dlg)
{
  Game* game = TheGame::Instance();
  GameState* s = game->GetState();
  dlg->SetPrevState(s);
  game->SetCurrentState(dlg); 
}

static void DialogOnOk()
{
  Game* game = TheGame::Instance();
  GameState* s = game->GetState();
  Dialog* dlg = dynamic_cast<Dialog*>(s);
  Assert(dlg);
  dlg->SetResult((int)AMJU_OK);
  dlg->GetDataFromGui(); // load GUI strings into member variables
  dlg->OnOk();
  dlg->Close();
}

static void DialogOnNo()
{
  Game* game = TheGame::Instance();
  GameState* s = game->GetState();
  Dialog* dlg = dynamic_cast<Dialog*>(s);
  Assert(dlg);
  dlg->SetResult((int)AMJU_NO);
  dlg->GetDataFromGui(); // load GUI strings into member variables
  dlg->Close();
}

static void DialogOnCancel()
{
  Game* game = TheGame::Instance();
  GameState* s = game->GetState();
  Dialog* dlg = dynamic_cast<Dialog*>(s);
  Assert(dlg);
  dlg->SetResult((int)AMJU_CANCEL);
  dlg->Close();
}

Dialog::Dialog()
{
  m_prevState = nullptr;
  m_onFinished = nullptr;
  m_result = (int)AMJU_RESULT_NOT_SET;
  m_drag = false;
}
 
void Dialog::SetTitle(const std::string& title)
{
  m_title = title;
}

void Dialog::SetGuiFilename(const std::string& filename)
{
  m_guiFilename = filename;
}

int Dialog::GetResult() const
{
  return m_result;
}

void Dialog::SetResult(int result) 
{
  m_result = result;
}

void Dialog::SetPrevState(GameState* s)
{
  m_prevState = s;
}

void Dialog::SetFinishCallback(DialogFinishCallback cb)
{
  m_onFinished = cb;
}

void Dialog::Draw()
{
  m_prevState->Draw();
}

void Dialog::Draw2d()
{
  m_prevState->Draw2d();
  m_gui->Draw();
  TheCursorManager::Instance()->Draw();
}

void Dialog::Update()
{
  m_gui->Update();
}

void Dialog::OnActive()
{
std::cout << "This dialog: " << m_guiFilename << " is activating...\n";

  GameState::OnActive();

  m_gui = LoadGui(m_guiFilename, false); // don't add as listener 
  Assert(m_gui);
  // GUI should get events through this game state's handler functions
  //  - don't automatically add to EventPoller

  // Set GUI text boxes etc. from member variables
  SetDataToGui();

  if (!m_title.empty())
  {
    GuiDialog* dlg = dynamic_cast<GuiDialog*>(m_gui.GetPtr());
    if (dlg)
    {
      dlg->SetTitle(m_title);
    }
  }

  // Set OK and cancel buttons
  GuiButton* ok = (GuiButton*)m_gui->GetElementByName("ok-button");
  Assert(ok);
  ok->SetCommand(DialogOnOk);

  GuiButton* no = (GuiButton*)m_gui->GetElementByName("no-button");
  if (no)
  {
    no->SetCommand(DialogOnNo);
  }

  GuiButton* cancel = (GuiButton*)m_gui->GetElementByName("cancel-button");
  Assert(cancel);
  cancel->SetCommand(DialogOnCancel);
}

void Dialog::OnDeactive()
{
  GameState::OnDeactive();

std::cout << "This dialog: " << m_guiFilename << " is deactivating...\n";
 
// TODO WHY NOT
//  m_gui = 0;
}

void Dialog::Close()
{
  Assert(m_result != (int)AMJU_RESULT_NOT_SET);

  TheGame::Instance()->SetCurrentState(m_prevState);

  // Call the callback function to say this dialog has finished
  Assert(m_onFinished);
  m_onFinished(this);
}

void Dialog::SetButtonText(const std::string& buttonName, const std::string& text)
{
  GuiButton* b = dynamic_cast<GuiButton*>(m_gui->GetElementByName(buttonName));
  if (b)
  {
    b->SetText(text);
  }
  else
  {
    std::cout << "Dialog: set button text failed for button \"" << buttonName << "\"\n";
  }
}

bool Dialog::OnKeyEvent(const KeyEvent& e) 
{ 
  Assert(m_gui);
  m_gui->OnKeyEvent(e);
  return true; 
}

bool Dialog::OnCursorEvent(const CursorEvent& e) 
{ 
  Assert(m_gui);

  if (m_drag)
  {
    Vec2f pos = m_gui->GetLocalPos();
    pos += Vec2f(e.dx, e.dy);
    m_gui->SetLocalPos(pos);
  }

  m_gui->OnCursorEvent(e);
  return true; 
}

bool Dialog::OnRotationEvent(const RotationEvent& e) 
{
  Assert(m_gui);
  m_gui->OnRotationEvent(e);
  return true; 
}

bool Dialog::OnJoyAxisEvent(const JoyAxisEvent& e) 
{ 
  Assert(m_gui);
  m_gui->OnJoyAxisEvent(e);
  return true; 
}

bool Dialog::OnButtonEvent(const ButtonEvent& e) 
{ 
  Assert(m_gui);
  m_gui->OnButtonEvent(e);
  return true; 
}

bool Dialog::OnMouseButtonEvent(const MouseButtonEvent& e) 
{ 
  Assert(m_gui);

  // Check if title bar clicked
  m_drag = false;
  GuiDialog* dlg = dynamic_cast<GuiDialog*>(m_gui.GetPtr());
  if (dlg->HasTitleBar() && e.isDown)
  {
    if (GetRect(dlg->GetTitleBar()).IsPointIn(Vec2f(e.x, e.y)))
    {
      std::cout << "Title bar clicked!\n";
      m_drag = true;
    }
  }

  m_gui->OnMouseButtonEvent(e);
  return true; 
}

bool Dialog::OnDoubleClickEvent(const DoubleClickEvent& e) 
{ 
  Assert(m_gui);
  m_gui->OnDoubleClickEvent(e);
  return true; 
}

bool Dialog::OnBalanceBoardEvent(const BalanceBoardEvent& e) 
{
  Assert(m_gui);
  m_gui->OnBalanceBoardEvent(e);
  return true; 
}

bool Dialog::OnTextEvent(const TextEvent& e) 
{
  Assert(m_gui);
  m_gui->OnTextEvent(e);
  return true; 
}

MessageBox::MessageBox()
{
  m_guiFilename = "gui-message-box.txt";
}

void MessageBox::GetDataFromGui() 
{
}

void MessageBox::SetDataToGui() 
{
  GuiText* t = (GuiText*)m_gui->GetElementByName("message-text");
  Assert(t);
  t->SetText(m_message);
}

void MessageBox::SetMessage(const std::string& message)
{
  m_message = message;
}

FileDialog::FileDialog()
{
}

void FileDialog::GetDataFromGui() 
{
  GuiText* t = (GuiText*)m_gui->GetElementByName("fd-path-text");
  Assert(t);
  m_filepath = t->GetText(); 
}

void FileDialog::SetDataToGui() 
{
  GuiFileDialog* fd = (GuiFileDialog*)m_gui->GetElementByName("my_file_dialog");
  Assert(fd);
  fd->SetPathAndFile(m_filepath);
  // Need this?
  fd->OnPathChange(); // ?
}

const std::string& FileDialog::GetFilePath() const
{
  return m_filepath;
}

void FileDialog::SetPath(const std::string& path)
{
  m_filepath = path;
}

}

