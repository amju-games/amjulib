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

  void SetText(const std::string& text);
  void SetTextSize(float textSize);

  Font* GetFont();

  void SetInverse(bool inv);
  void SetDrawBg(bool drawBg);

  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };

  void SetJust(Just j);

protected:
  std::string m_text;
  std::string m_fontName;
  Just m_just;
  float m_textWidth;
  bool m_inverse;
  bool m_drawBg;
  PFont m_font;
  float m_textSize;
};
}

#endif
