#ifndef GUI_TEXT_EDIT_H
#define GUI_TEXT_EDIT_H

#include "GuiText.h"
#include "GuiRect.h"

namespace Amju
{
// Interface for different flavours of text edit controls
class IGuiTextEdit
{
public:
  IGuiTextEdit() : m_onChangeFunc(nullptr) {}
  virtual ~IGuiTextEdit() {}
  
  virtual void SetText(const std::string& text) = 0;
  virtual std::string GetText() const = 0;
  
  // Set callback which is called whenever text is changed (e.g. a new char is typed)
  void SetOnChangeFunc(CommandFunc onCharFunc);
  
protected:
  CommandFunc m_onChangeFunc;
  
};
  
class GuiTextEdit : public IGuiTextEdit, public GuiText
{
public:
  static const char* NAME;

  GuiTextEdit();

  GuiTextEdit* Clone() override { return new GuiTextEdit(*this); }

  void Draw() override; // Adds focus border and caret
  bool Load(File*) override; 
  bool OnKeyEvent(const KeyEvent&) override; 
  bool OnCursorEvent(const CursorEvent&) override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnTextEvent(const TextEvent&) override;
  void SetText(const std::string&) override;
  std::string GetText() const override { return GuiText::GetText(); }
  
  void Insert(char);

  void SetIsPassword(bool isPassword);
  bool IsPassword() const;

protected:
  void GetFirstLast(int line, int* first, int* last) override; 
  int CalcCursorPos(float mouseX);
  int NextWord(int); // for caret/selection movement
  int PrevWord(int);
 
protected:
  float m_caretTimer;
  bool m_drawCaret;
  bool m_isPassword; // if so, don't show characters
  GuiRect m_rect; // focus rect
};
}

#endif
