#ifndef GS_TODAYSTASK_H_INCLUDED
#define GS_TODAYSTASK_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSTodaysTask : public GSGui
{
  GSTodaysTask();
  friend class Singleton<GSTodaysTask>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSTodaysTask> TheGSTodaysTask;
} // namespace
#endif
