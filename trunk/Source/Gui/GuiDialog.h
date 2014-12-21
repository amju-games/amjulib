#ifndef GUI_DIALOG_H_INCLUDED
#define GUI_DIALOG_H_INCLUDED

#include "GuiWindow.h"
#include "GuiText.h"

namespace Amju
{
class GuiDialog : public GuiWindow
{
public:
  static const char* NAME;
 
  GuiDialog();
  virtual void Draw() override;
  virtual bool Load(File*) override;

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnCursorEvent(const CursorEvent&) override; 

  void SetTitle(const std::string&);
  void SetHasTitleBar(bool);
  bool HasTitleBar() const;
  GuiText* GetTitleBar();

protected:
  GuiText m_title;
  bool m_hasTitleBar;
  bool m_drag; // we can position the dialog box by dragging the title bar
};
}

#endif

