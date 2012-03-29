#ifndef GUI_LIST_BOX_H_INCLUDED
#define GUI_LIST_BOX_H_INCLUDED

#include <set>
#include "GuiComposite.h"
#include "GuiText.h"

namespace Amju
{
class GuiListBox : public GuiComposite
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

protected:
  bool m_isMultiSelect;
  typedef std::set<int> SelSet;
  SelSet m_selset;
};
}

#endif

