#ifndef GUI_DIALOG_H_INCLUDED
#define GUI_DIALOG_H_INCLUDED

#include "GuiWindow.h"
#include "GuiText.h"

namespace Amju
{
class GuiDialog : public GuiComposite
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
  void SetTitleBarColour(const Colour& c) { m_titleColour = c; }
  void SetBgColour(const Colour& c) { m_bgColour = c; }

protected:
  GuiText m_title;
  bool m_hasTitleBar;
  bool m_drag; // we can position the dialog box by dragging the title bar
  Colour m_bgColour;
  Colour m_titleColour;
};
}

#endif

