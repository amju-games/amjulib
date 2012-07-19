#ifndef GUI_LIST_BOX_H_INCLUDED
#define GUI_LIST_BOX_H_INCLUDED

#include <set>
#include "GuiWindow.h"
#include "GuiText.h"

namespace Amju
{
class GuiListBox : public GuiWindow
{
public:
  static const char* NAME;

  GuiListBox();

  void AddItem(GuiText* text);  

  virtual void Draw();
  virtual bool Load(File*);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent& mbe);

  void SetSelected(int child, bool selected);
  bool IsSelected(int child) const;
  int GetSelectedItem() const; // only for non-multi-select lists

  void SetIsMultiSel(bool isMulti);
  bool IsMultiSel() const;

protected:
  bool m_isMultiSelect;
  typedef std::set<int> SelSet;
  SelSet m_selset;
};
}

#endif

