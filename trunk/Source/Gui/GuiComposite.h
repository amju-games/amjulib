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

  GuiComposite();
  virtual void Draw();
  virtual bool Load(File*); 
  virtual GuiElement* GetElementByName(const std::string& name);

  // Up/Down to set focus to prev/next child
  virtual bool OnKeyEvent(const KeyEvent&); 

  bool SetFocusPrevChild();
  bool SetFocusNextChild();

  int GetNumChildren() const;
  GuiElement* GetChild(int i);
  virtual void AddChild(GuiElement*);
  void Clear();

protected:
  bool LoadChildren(File*);

  GuiElements m_children;
};
}

#endif

