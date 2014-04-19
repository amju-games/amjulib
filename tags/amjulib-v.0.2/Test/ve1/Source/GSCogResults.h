#ifndef GS_COGRESULTS_H_INCLUDED
#define GS_COGRESULTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSCogResults : public GSGui
{
  GSCogResults();
  friend class Singleton<GSCogResults>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCogResults> TheGSCogResults;
} // namespace
#endif
