#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include <string>
#include "GuiElement.h"
#include "Font.h"
#include <Colour.h>
#include <StringUtils.h>

namespace Amju
{
// This interface is so we can include platform-specific text widgets,
//  where get/set text is the only operation we can be sure of.
class IGuiText
{
public:
  virtual ~IGuiText() {}
  virtual void SetText(const std::string& text) = 0;
  virtual std::string GetText() const = 0;
};

// ** GuiTextBase **
// Useful members for GuiElement text-based subclasses
class GuiTextBase : public IGuiText, public GuiElement
{
public:
  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };

  // Set the scale for fonts.
  // A character at font size 1 takes up 1/20th of the screen height.
  // (1/20 because entire height is -1..1, i.e. distance of 2)
  static constexpr float CHAR_HEIGHT_FOR_SIZE_1 = .1f;

  std::string GetText() const override { return m_text; }

  virtual void SetJust(Just j) { m_just = j; }

  Font* GetFont();
  void SetFont(Font*);
  void SetFont(const std::string& fontName);

  virtual void SetIsMulti(bool multi); // Set to true for multi-line text
  bool IsMulti() const;
  int GetNumLines() const;

  // Foreground colour - i.e. colour of text glyphs
  void SetFgCol(const Colour& col);
  const Colour& GetFgCol() const;
  // Background colour - i.e. colour behind the text
  void SetBgCol(const Colour& col);
  const Colour& GetBgCol() const;

  bool Load(File*) override;
  bool Save(File*) override;

  // Load type-specific info
  virtual bool LoadTextInfo(File*) { return true; }
  // Save type-specific info
  virtual bool SaveTextInfo(File*) { return true; }

protected:
  bool ParseFontInfoLine(File* f);

  // Handle single-string attribute when loading 
  //  (and potentially when setting properties)
  // Overridden impls: call this first, then handle more specific attribs if not
  //  handled by this impl (i.e. false is returned).
  virtual bool HandleAttrib(const std::string& s);

  // Handle key=value pair when loading 
  //  (and potentially when setting properties)
  virtual bool HandleAttrib(const std::string& key, const std::string& value);

  // Replace special chars and utf-8 sequences 
  std::string ReplaceEscaped(const std::string& escapedText);

protected:
  std::string m_text; // Text after localisation look up 
  std::string m_preLocalisedText; // Text before look up, so containing $$$ codes
  std::string m_fontPathFilename;
  std::string m_fontName;
  PFont m_font;
  Just m_just = AMJU_JUST_LEFT;
  float m_textSize = 1.f; // "point" size
  bool m_isMulti = false; // true for multi-line text
  Strings m_lines; // for multi-line text, this is m_text split into lines
  Colour m_fgCol; // foreground colour, i.e. colour of the text
  Colour m_bgCol; // background colour
};
 
class GuiText : public GuiTextBase
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiText* Clone() override { return new GuiText(*this); }

  GuiText();
  virtual void Draw() override;
  virtual void TextToSpeech() override;

  Texture* GetTexture() override;

  // Load just the text info (not name, size, etc., which is loaded in
  //  GuiElement::Load.)
  bool LoadTextInfo(File*) override;
  bool SaveTextInfo(File*) override;

  virtual void SetText(const std::string& text) override;

  void SetJust(Just j) override;

  // Really font size 
  void SetFontSize(float textSize);
  float GetFontSize() const;
  
  void SetIsMulti(bool) override; // Set to true for multi-line text

  // Set X scale, to squish more text into a tight squeeze, etc
  void SetScaleX(float scaleX);
  float GetScaleX() const;

  // Calc size for SizeToText, but don't actually resize
  Vec2f CalcSizeToText() const;

  // Change size to fit text exactly
  void SizeToText();

  // Call to decide which bits of the text fit in the bounding rect
  void RecalcFirstLast();

  void SetInverse(bool inv);
  void SetDrawBg(bool drawBg);

  // For text which is revealed a char at a time, this is the period for each char
  void SetCharTime(float secs);

protected:
  // Handle single-string attribute when loading (and potentially when setting properties)
  bool HandleAttrib(const std::string& s) override;

  // Handle key=value pair when loading (and potentially when setting properties)
  bool HandleAttrib(const std::string& key, const std::string& value) override;

  // calc first and last char to draw in line
  virtual void GetFirstLast(int line, int* first, int* last); 

  friend struct WidthFinder;
  float GetTextWidth(const std::string& text) const;

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
  
  bool m_inverse;
  bool m_drawBg;

  float m_charTime; // time to wait between drawing chars
  float m_currentCharTime; // show one more char when we reach m_charTime
  int m_currentChar; // index of final char to draw

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
