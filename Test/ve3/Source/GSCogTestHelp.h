#ifndef GS_COGTESTHELP_H_INCLUDED
#define GS_COGTESTHELP_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSCogTestHelp : public GSGui
{
  GSCogTestHelp();
  friend class Singleton<GSCogTestHelp>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCogTestHelp> TheGSCogTestHelp;
} // namespace
#endif
