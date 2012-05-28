#ifndef GS_COGTESTMENU_H_INCLUDED
#define GS_COGTESTMENU_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSCogTestMenu : public GSGui
{
  GSCogTestMenu();
  friend class Singleton<GSCogTestMenu>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCogTestMenu> TheGSCogTestMenu;
} // namespace
#endif
