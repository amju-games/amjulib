#ifndef GS_ACHIEVEMENTS_H_INCLUDED
#define GS_ACHIEVEMENTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSAchievements : public GSGui
{
  GSAchievements();
  friend class Singleton<GSAchievements>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSAchievements> TheGSAchievements;
} // namespace
#endif
