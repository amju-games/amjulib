#ifndef GS_ADMINMENU_H_INCLUDED
#define GS_ADMINMENU_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSAdminMenu : public GSGui
{
  GSAdminMenu();
  friend class Singleton<GSAdminMenu>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSAdminMenu> TheGSAdminMenu;
} // namespace
#endif
