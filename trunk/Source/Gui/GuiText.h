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
  static const float CHAR_HEIGHT_FOR_SIZE_1;

  GuiText();
  virtual void Draw();  
  virtual bool Load(File*); 
  virtual void TextToSpeech();

  // Load just the text info (not name, size, etc)
  bool LoadText(File*);

  virtual void SetText(const std::string& text);
  virtual const std::string& GetText() const;

  // Really font size 
  void SetFontSize(float textSize);
  float GetFontSize() const;
  
  // Set X scale, to squish more text into a tight squeeze, etc
  void SetScaleX(float scaleX);
  float GetScaleX() const;

  void SizeToText(); // changes size to accomodate text

  // Call to decide which bits of the text fit in the bounding rect
  void RecalcFirstLast();

  Font* GetFont();
  void SetFont(Font*);
  void SetFont(const std::string& fontName);

  void SetInverse(bool inv);
  void SetDrawBg(bool drawBg);
  void SetFgCol(const Colour& col);
  void SetBgCol(const Colour& col);

  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };

  void SetJust(Just j);
  void SetIsMulti(bool); // Multi line ?
  bool IsMulti() const;

  int GetNumLines() const;

  // For text which is revealed a char at a time, this is the period for each char
  void SetCharTime(float secs);

protected:
  // calc first and last char to draw in line
  virtual void GetFirstLast(int line, int* first, int* last); 

  friend struct WidthFinder;
  float GetTextWidth(const std::string& text);

  // Called twice from Draw, for drop shadow
  void ReallyDraw();

  void DrawSingleLine(bool selected, int first, int last, const Colour& fg, const Colour& bg);
  void DrawMultiLine(const Colour& fg, const Colour& bg);

  void PrintLine(const std::string&, float x, float y);

protected:
  typedef std::vector<RCPtr<TriList> > TriLists;
  RCPtr<TriList> m_triList; // simplest case - one line only
  TriLists m_triLists; // for multi-line text
  RCPtr<TriList> m_triListCaret; 
  RCPtr<TriList> m_triListSelection; 
  
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

  float m_scaleX;
};
}

#endif
