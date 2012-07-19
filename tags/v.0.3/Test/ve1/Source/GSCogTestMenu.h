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

  void ResetTests();

  // TODO Set order of tests; set propoerties of each test

private:
  int m_nextTest;
};
typedef Singleton<GSCogTestMenu> TheGSCogTestMenu;
} // namespace
#endif
