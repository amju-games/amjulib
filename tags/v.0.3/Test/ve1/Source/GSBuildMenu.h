#ifndef GS_BUILDMENU_H_INCLUDED
#define GS_BUILDMENU_H_INCLUDED

#include <Singleton.h>
#include "GSMain.h"

namespace Amju 
{
class GSBuildMenu : public GSMain
{
  GSBuildMenu();
  friend class Singleton<GSBuildMenu>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSBuildMenu> TheGSBuildMenu;
} // namespace
#endif
