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
  
  void AdvanceToNextTest();

  void ResetTests();

  // Practice mode
  void AskPractice();
  bool IsPrac() const; // true if player wants a practice
  void SetIsPrac(bool);
  void DoPractice(bool b);
  // TODO Set order of tests; set propoerties of each test

  // Returns true while we are doing the tests. We go back to GSMain then back to here
  //  so we can collect rewards between tests.
  bool IsDoingTests();

private:
  int m_nextTest;
  CommandFunc m_func; // call to go to next state... could just use a switch...?
  bool m_isPrac;
};
typedef Singleton<GSCogTestMenu> TheGSCogTestMenu;
} // namespace
#endif
