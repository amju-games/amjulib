#ifndef GUI_KEYBOARD_H_INCLUDED
#define GUI_KEYBOARD_H_INCLUDED

#include "GuiWindow.h"

namespace Amju
{
// Generates keyboard events
class GuiKeyboard : public GuiWindow
{
public:
  static const char* NAME;

  GuiKeyboard();
  virtual bool Load(File*);
  virtual void Draw();

  // Set Keyboard scale
  void SetKbScale(float s);

  // Set distance between keys
  void SetKbGaps(float g);

  virtual bool OnKeyEvent(const KeyEvent&); 

protected:
  void SetKeys();

  // KB has multiple modes - e.g.letter mode, number/punctuation mode, symbol mode.
  // Mode name corresponds to a group in the GUI text file.
  void ChangePage(int page);

protected:
  // Page - letters, num, symbol 
  int m_page;  
};
}

#endif

