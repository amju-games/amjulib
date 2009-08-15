#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include <string>
#include "GuiElement.h"

namespace Amju
{
class GuiText : public GuiElement
{
public:
  static const char* NAME;

  virtual void Draw();  
  virtual bool Load(File*); 

protected:
  std::string m_text;
};
}

#endif
