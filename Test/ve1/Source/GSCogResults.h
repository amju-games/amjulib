#ifndef GS_COG_RESULTS_H_INCLUDED
#define GS_COG_RESULTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
// Shows a piece of storyline. No time to be nice, just show some text.
class GSCogResults : public GSGui
{
  GSCogResults();
  friend class Singleton<GSCogResults>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

private:
};
typedef Singleton<GSCogResults> TheGSCogResults;

} // namespace

#endif
