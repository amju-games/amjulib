#ifndef GUI_TEXT_EDIT_H
#define GUI_TEXT_EDIT_H

#include "GuiText.h"

namespace Amju
{
class GuiTextEdit : public GuiText
{
public:
  static const char* NAME;

  GuiTextEdit();

  virtual void Draw();  
  virtual bool Load(File*); 
  virtual bool OnKeyEvent(const KeyEvent&); 
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  virtual void SetText(const std::string&);
  virtual const std::string& GetText() const;

  void Insert(char);

protected:
  virtual void GetFirstLast(int line, int* first, int* last); 
  int CalcCursorPos(float mouseX);
 
protected:
  float m_caretTimer;
  bool m_drawCaret;
  std::string m_myText;
};
}

#endif
