#ifndef GUI_COMPOSITE_H
#define GUI_COMPOSITE_H

#include <vector>
#include "GuiElement.h"

namespace Amju
{
class GuiComposite : public GuiElement
{
public:
  static const char* NAME;

  virtual void Draw();
  virtual bool Load(File*); 
  virtual GuiElement* GetElementByName(const std::string& name);

  int GetNumChildren() const;
  GuiElement* GetChild(int i);
  void Clear();

protected:
  GuiElements m_children;
};
}

#endif

