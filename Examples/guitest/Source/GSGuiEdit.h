#ifndef GS_GUIEDIT_H_INCLUDED
#define GS_GUIEDIT_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSGuiEdit : public GSGui
{
  GSGuiEdit();
  friend class Singleton<GSGuiEdit>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSGuiEdit> TheGSGuiEdit;
} // namespace
#endif
