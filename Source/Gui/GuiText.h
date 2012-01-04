#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include <string>
#include "GuiElement.h"
#include "Font.h"
#include <Colour.h>
#include <StringUtils.h>

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
  void SetIsMulti(bool); // Multi line ?

  void SetCharTime(float secs);

protected:
  // calc first and last char to draw in line
  virtual void GetFirstLast(int line, int* first, int* last); 

  friend struct WidthFinder;
  float GetTextWidth(const std::string& text);

  void DrawSingleLine();
  void DrawMultiLine();

  void PrintLine(const std::string&, float y);

protected:
  std::string m_text;
  std::string m_fontName;
  Just m_just;
  bool m_inverse;
  bool m_drawBg;
  PFont m_font;
  float m_textSize;
  Colour m_bgCol;
  Colour m_fgCol;

  float m_charTime; // time to wait between drawing chars
  float m_currentCharTime; // show one more char when we reach m_charTime
  int m_currentChar; // index of final char to draw

  bool m_isMulti;
  Strings m_lines; // for multi-line text boxes, split the text into lines
  int m_topLine; // first line displayed
};
}

#endif
