#pragma once

#include <GameState.h>
#include <GuiDialog.h>

namespace Amju
{
class Dialog;
typedef RCPtr<Dialog> PDialog;

void DoModalDialog(Dialog* dlg);

// Type of function called when dialog is finished
typedef void (*DialogFinishCallback)(Dialog*);

enum 
{ 
  AMJU_RESULT_NOT_SET = -1, 
  AMJU_OK = 0, 
  AMJU_YES = 0,
  AMJU_NO = 1,
  AMJU_CANCEL = 2 
};

class Dialog : public GameState
{
public:
  Dialog();

  void SetGuiFilename(const std::string&);

  void SetPrevState(GameState*);

  // Set one callback which is called whatever button closes the dialog
  void SetFinishCallback(DialogFinishCallback);

  virtual void Draw() override;
  virtual void Draw2d() override;
  virtual void Update() override;
  virtual void OnActive() override;
  virtual void OnDeactive() override;

  void Close();

  // Get button used to close the dialog, e.g. AMJU_OK or AMJU_CANCEL
  int GetResult() const;
  void SetResult(int);

  void SetButtonText(const std::string& buttonName, const std::string& text);
  void SetTitle(const std::string& title);

  virtual void GetDataFromGui() = 0;
  virtual void SetDataToGui() = 0;

  virtual bool OnKeyEvent(const KeyEvent&) override; 
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnRotationEvent(const RotationEvent&) override;
  virtual bool OnJoyAxisEvent(const JoyAxisEvent&) override;
  virtual bool OnButtonEvent(const ButtonEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnDoubleClickEvent(const DoubleClickEvent&) override;
  virtual bool OnBalanceBoardEvent(const BalanceBoardEvent&) override;
  virtual bool OnTextEvent(const TextEvent&) override;
  
  virtual void OnOk() {}

protected:
  GameState* m_prevState;
  PGuiElement m_gui;
  std::string m_guiFilename;
  DialogFinishCallback m_onFinished;
  int m_result;
  std::string m_title;
  bool m_drag;
};

class MessageBox : public Dialog
{
public:
  MessageBox();
  virtual void GetDataFromGui() override;
  virtual void SetDataToGui() override;

  void SetMessage(const std::string& message);

protected:
  std::string m_message;
};

class FileDialog : public Dialog
{
public:
  FileDialog();
  
  virtual void GetDataFromGui() override;
  virtual void SetDataToGui() override;

  // Get selected file with path
  const std::string& GetFilePath() const;

  // Set path for dialog to start in
  void SetPath(const std::string&);

protected:
  std::string m_filepath;
};
}

