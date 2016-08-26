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
  virtual ~GuiComposite();

  virtual void Draw() override;
  virtual bool Load(File*) override;
  virtual GuiElement* GetElementByName(const std::string& name) override;

  // Up/Down to set focus to prev/next child
  virtual bool OnKeyEvent(const KeyEvent&) override;
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnRotationEvent(const RotationEvent&) override;
  virtual bool OnJoyAxisEvent(const JoyAxisEvent&) override;
  virtual bool OnButtonEvent(const ButtonEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnDoubleClickEvent(const DoubleClickEvent&) override;
  virtual bool OnBalanceBoardEvent(const BalanceBoardEvent&) override;
  virtual bool OnTextEvent(const TextEvent&) override;
  virtual bool OnQuitEvent() override;
  virtual bool OnResizeEvent(const ResizeEvent&) override;

  bool SetFocusPrevChild();
  bool SetFocusNextChild();

  int GetNumChildren() const;
  GuiElement* GetChild(int i);
  virtual void AddChild(GuiElement*);

  virtual void Clear();

  // Calc size from union of size of all descendents
  void SetSizeFromChildren();

  // Set visibility for self and children
  virtual void SetAncestorsVisible(bool ancestorVis) override;
  
protected:
  bool LoadChildren(File*);
  bool LoadOneChild(File*);
  
  GuiElements m_children;
};
}

#endif

