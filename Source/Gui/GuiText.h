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

  Font* GetFont();

protected:
  std::string m_text;
  std::string m_fontName;
  enum Just { AMJU_JUST_LEFT, AMJU_JUST_RIGHT, AMJU_JUST_CENTRE };
  Just m_just;
  float m_textWidth;
};
}

#endif
