#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include <string>
#include "GuiElement.h"
#include "Font.h"

namespace Amju
{
class GuiText : public GuiElement
{
public:
  static const char* NAME;

  GuiText();
  virtual void Draw();  
  virtual bool Load(File*); 

  // Load just the text info (not name, size, etc)
  bool LoadText(File*);

  void SetText(const std::string& text);
  void SetTextSize(float textSize);

  Font* GetFont();

  void SetInverse(bool inv);
  void SetDrawBg(bool drawBg);

  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };

  void SetJust(Just j);

protected:
  // calc first and last char to draw in line
  virtual void GetFirstLast(int line, int* first, int* last); 

  float GetTextWidth(const std::string& text);

protected:
  std::string m_text;
  std::string m_fontName;
  Just m_just;
  bool m_inverse;
  bool m_drawBg;
  PFont m_font;
  float m_textSize;
};
}

#endif
