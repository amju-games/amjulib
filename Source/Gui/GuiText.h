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
  virtual void TextToSpeech();

  // Load just the text info (not name, size, etc)
  bool LoadText(File*);

  virtual void SetText(const std::string& text);
  virtual const std::string& GetText() const;
 
  void SetTextSize(float textSize);
  float GetTextSize() const;

  void SizeToText(); // changes size to accomodate text

  // Call to decide which bits of the text fit in the bounding rect
  void RecalcFirstLast();

  Font* GetFont();

  void SetInverse(bool inv);
  void SetDrawBg(bool drawBg);
  void SetFgCol(const Colour& col);
  void SetBgCol(const Colour& col);

  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };

  void SetJust(Just j);
  void SetIsMulti(bool); // Multi line ?

  void SetCharTime(float secs);

protected:
  // calc first and last char to draw in line
  virtual void GetFirstLast(int line, int* first, int* last); 

  friend struct WidthFinder;
  float GetTextWidth(const std::string& text);

  void DrawSingleLine(int first, int last, const Colour& fg, const Colour& bg);
  void DrawMultiLine();

  void PrintLine(const std::string&, float x, float y);

protected:
  static const float CHAR_HEIGHT_FOR_SIZE_1;

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

  // First and last chars drawn in line (single line only)
  int m_first;
  int m_last;

  int m_caret; // index of caret: 0 means at left
  int m_selectedText; // index of other end of selected text (poss before or after caret)
};
}

#endif
