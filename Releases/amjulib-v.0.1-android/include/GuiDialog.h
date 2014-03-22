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
  virtual void Draw();
  virtual bool Load(File*);

  void SetTitle(const std::string&);
  void SetHasTitleBar(bool);
  bool HasTitleBar() const;
  GuiText* GetTitleBar();

protected:
  GuiText m_title;
  bool m_hasTitleBar;
};
}

#endif

