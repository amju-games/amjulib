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
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnRotationEvent(const RotationEvent&);
  virtual bool OnJoyAxisEvent(const JoyAxisEvent&);
  virtual bool OnButtonEvent(const ButtonEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  bool SetFocusPrevChild();
  bool SetFocusNextChild();

  int GetNumChildren() const;
  GuiElement* GetChild(int i);
  virtual void AddChild(GuiElement*);

  virtual void Clear();

protected:
  bool LoadChildren(File*);

  GuiElements m_children;
};
}

#endif

