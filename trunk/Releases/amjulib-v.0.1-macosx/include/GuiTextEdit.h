#ifndef GUI_TEXT_EDIT_H
#define GUI_TEXT_EDIT_H

#include "GuiText.h"
#include "GuiRect.h"

namespace Amju
{
class GuiTextEdit : public GuiText
{
public:
  static const char* NAME;

  GuiTextEdit();

  virtual void Draw(); // Adds focus border and caret
  virtual bool Load(File*); 
  virtual bool OnKeyEvent(const KeyEvent&); 
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  virtual void SetText(const std::string&);

  void Insert(char);

  // Set callback which is called whenever text is changed (e.g. a new char is typed)
  void SetOnChangeFunc(CommandFunc onCharFunc);

  void SetIsPassword(bool isPassword);
  bool IsPassword() const;

protected:
  virtual void GetFirstLast(int line, int* first, int* last); 
  int CalcCursorPos(float mouseX);
  int NextWord(int); // for caret/selection movement
  int PrevWord(int);
 
protected:
  float m_caretTimer;
  bool m_drawCaret;
  CommandFunc m_onChangeFunc;
  bool m_isPassword; // if so, don't show characters
  GuiRect m_rect; // focus rect
};
}

#endif
