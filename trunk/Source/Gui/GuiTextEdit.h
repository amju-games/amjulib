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

  std::string GetText() const;

  void Insert(char);

protected:
  virtual void GetFirstLast(int line, int* first, int* last); 

protected:
  int m_caret; // index of caret: 0 means at left
  float m_caretTimer;
  std::string m_myText;
};
}

#endif
