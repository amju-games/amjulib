#ifndef TEXT_DRAW_H 
#define TEXT_DRAW_H 

#include "GuiElement.h"
#include "SceneNode.h"

namespace Amju
{
class TextDraw 
{
public:
  
  void Update();
  void Draw();

  // Create scene from text
  void CreateText(const std::string& text);

protected:
};
}

#endif
