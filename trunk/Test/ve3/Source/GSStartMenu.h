#ifndef GS_STARTMENU_H_INCLUDED
#define GS_STARTMENU_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStartMenu : public GSGui
{
  GSStartMenu();
  friend class Singleton<GSStartMenu>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSStartMenu> TheGSStartMenu;
} // namespace
#endif
